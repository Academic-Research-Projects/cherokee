#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include <sys/time.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include "http/http_request/http_request.h"
#include "crud_operations/http_put.h"

extern void *http_put(HttpRequest *request, int client_socket);

// Test case for successful file creation
Test(http_put, file_creation_success) {
    // Create a test file request
    HttpRequest *request =  malloc(sizeof(HttpRequest));
    request->body = strdup("This is a test file.");

    // Generate a unique filename based on the current timestamp
    struct timeval tv;
    gettimeofday(&tv, NULL);
    long long milliseconds = (long long)(tv.tv_sec) * 1000 + (long long)(tv.tv_usec) / 1000;
    char filename[256];
    snprintf(filename, sizeof(filename), "/test_unit_%lld.txt", milliseconds);

    request->request_line.requestTarget = strdup(filename);

    // Create a pipe
    int pipefd[2];
    pipe(pipefd);

    // Call the http_put function
    http_put(request, pipefd[1]);

    // Close the write end of the pipe
    close(pipefd[1]);

    // Read the response from the pipe
    char response[1024];
    int status_code;
    read(pipefd[0], response, sizeof(response));
    sscanf(response, "HTTP/1.1 %d", &status_code);

    // Check that the response is a 200 OK
    cr_assert_eq(status_code, 200, "Response does not match expected value");

    // Close the read end of the pipe
    close(pipefd[0]);

    // Free allocated memory
    free(request->body);
    free(request->request_line.requestTarget);
    free(request);
}

// Test case for successful file update
Test(http_put, file_update_success) {
    // Create a test file request
    HttpRequest *request = malloc(sizeof(HttpRequest));
    request->body = strdup("This is an updated test file.");
    request->request_line.requestTarget = strdup("/test_unit.txt");

    // Create a pipe
    int pipefd[2];
    pipe(pipefd);

    // Call the http_put function
    http_put(request, pipefd[1]);

    // Close the write end of the pipe
    close(pipefd[1]);

    // Read the response from the pipe
    char response[1024];
    int status_code;
    read(pipefd[0], response, sizeof(response));
    sscanf(response, "HTTP/1.1 %d", &status_code);

    // Check that the response is a 200 OK
    cr_assert_eq(status_code, 200, "Response does not match expected value");

    // Close the read end of the pipe
    close(pipefd[0]);

    // Free allocated memory
    free(request->body);
    free(request->request_line.requestTarget);
    free(request);
}
