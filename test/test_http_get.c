#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include <unistd.h>
#include <criterion/assert.h>
#include <string.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "crud_operations/http_get.h"
#include "status_codes/http_status_codes.h"
#include "http/http_response/http_response.h"
#include "master/server_socket.h"

// The function to test
HttpResponse *createError404(HttpResponse *response);

Test(createError404_validResponse, validResponse)
{
    // Create an empty HttpResponse structure
    struct HttpResponse response;
    memset(&response, 0, sizeof(struct HttpResponse));

    // Call the createError404 function
    createError404(&response);

    // Assert that the response has the correct values
    cr_assert_str_eq(response.headers->name, "Content-Type");
    cr_assert_str_eq(response.headers->value, "text/plain");
    cr_assert_str_eq(response.httpVersion, "HTTP/1.1");
    cr_assert_eq(response.statusCode, 404);
    cr_assert_str_eq(response.statusText, "Not Found\n");
    cr_assert_str_eq(response.body, "Not Found\n");

    // Clean up any dynamically allocated resources, if applicable
    free(response.headers);
    free(response.body);
}

// Mock the write function to capture the response sent to the client
// ssize_t mock_write(int fd, const void *buf, size_t count)
// {
//     // Capture the response here or perform assertions on it
//     // You can redirect the write to a buffer or check the content directly
//     return count;
// }

Test(http_get, successful_file_retrieval)
{
    // Arrange
    int client_socket = open("request.txt", O_RDWR | O_CREAT | S_IRWXU | O_TRUNC, 0777); // Provide the client socket here
    char buffer[200];
    printf("unit_test_client_socket: %d\n", client_socket);

    // Create a mock HttpRequest with the request target of an existing file
    HttpRequest request;
    request.request_line.requestTarget = "file.txt";
    request.request_line.method = "GET";

    http_get(&request, client_socket);

    // Redirect the write function to our mock_write function
    // This allows us to capture the response sent to the client
    // // criterion_redirect_write(mock_write);

    // // Create the existing file for testing
    // FILE *file = fopen("test_files/sendhelp.txt", "w");
    // fprintf(file, "This is the content of the existing file.\n");
    // fclose(file);

    // // Act
    // void *result = http_get(&request, client_socket);

    // // Assert
    // cr_assert_null(result, "Expected http_get to return NULL");

    // We can perform further assertions on the captured response here
    // For example, check the response status code, content, headers, etc.

    // Clean up the created file
    // unlink("test_files/zoubisou.json");
}

// Test(createError404_null_response, nullResponse)
// {
//     // Call the createError404 function with a NULL response pointer
//     HttpResponse *response = createError404(NULL);

//     // Assert that the function returns NULL
//     cr_assert_null(response);
// }

// Test(createError404_empty_response, emptyResponse)
// {
//     // Create an empty HttpResponse structure
//     struct HttpResponse response;
//     memset(&response, 0, sizeof(struct HttpResponse));

//     // Call the createError404 function
//     createError404(&response);

//     // Assert that the response has the correct values
//     cr_assert_null(response.headers);
//     cr_assert_str_eq(response.httpVersion, "HTTP/1.1");
//     cr_assert_eq(response.statusCode, 404);
//     cr_assert_str_eq(response.statusText, "Not Found\n");
//     cr_assert_str_eq(response.body, "Not Found\n");
// }