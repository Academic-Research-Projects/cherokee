#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/time.h>
#include "http/http_formatter/http_formatter.h"
#include "http/http_parser/http_parser.h"
#include "http/http_response/http_response.h"
#include "http/http_request/http_request.h"
#include "status_codes/http_status_codes.h"
#include "crud_operations/http_post.h"

#define PORT 8080
#define BASE_DIRECTORY "test_files"
#define RESPONSE_BUFFER_SIZE 4096
#define FILE_PATH_SIZE 512
#define BUFFER_SIZE 1024

extern void *http_post(HttpRequest *request, int client_socket);

// Test case for successful file creation
Test(http_post, file_creation_success)
{
    // Create a test file request
    HttpRequest *request = malloc(sizeof(HttpRequest));
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

    // Call the http_post function
    http_post(request, pipefd[1]);

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
    free(request->body);
    free(request->request_line.requestTarget);
    free(request);
}

Test(http_post, file_already_exists)
{
    // Create a test file request
    HttpRequest *request = malloc(sizeof(HttpRequest));
    request->body = strdup("This is a test file.");
    request->request_line.requestTarget = strdup("/test.txt");

    // Create a pipe
    int pipefd[2];
    pipe(pipefd);

    // Call the http_post function twice to simulate a file already existing
    http_post(request, pipefd[1]);
    http_post(request, pipefd[1]);

    // Close the write end of the pipe
    close(pipefd[1]);

    // Read the response from the pipe
    char response[1024];
    int status_code;
    read(pipefd[0], response, sizeof(response));
    sscanf(response, "HTTP/1.1 %d", &status_code);

    // Check that the response is a 409 Conflict
    cr_assert_eq(status_code, 409, "Response does not match expected value");

    // Close the read end of the pipe
    close(pipefd[0]);
    free(request->body);
    free(request->request_line.requestTarget);
    free(request);
}
