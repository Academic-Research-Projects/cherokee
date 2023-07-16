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
#include "crud_operations/http_post.h"
#include "http/http_formatter/http_formatter.h"
#include "http/http_parser/http_parser.h"
#include "http/http_response/http_response.h"
#include "http/http_request/http_request.h"
#include "status_codes/http_status_codes.h"

#define PORT 8080
#define BASE_DIRECTORY "test_files"
#define RESPONSE_BUFFER_SIZE 4096
#define FILE_PATH_SIZE 512
#define BUFFER_SIZE 1024

void *http_post(HttpRequest *request, int client_socket);

// Test creating a new file with a valid request body
Test(http_post, create_new_file_with_request_body)
{
    // Redirect stdout to a buffer to capture the response
    char response_buffer[RESPONSE_BUFFER_SIZE];
    cr_redirect_stdout_to_buffer(response_buffer, RESPONSE_BUFFER_SIZE);

    // Create a test request
    HttpRequest request = {
        .body = "This is a test request body.",
        .request_line = {
            .method = "POST",
            .requestTarget = "/test.txt",
            .httpVersion = "HTTP/1.1"
        }
    };
    
    // Call the http_post function
    http_post(&request, 1);

    // Check that the file was created
    FILE *file_handle = fopen("test_files/test.txt", "r");
    cr_assert_not_null(file_handle, "File was not created");

    // Check that the file contents match the request body
    char file_contents[BUFFER_SIZE];
    fgets(file_contents, BUFFER_SIZE, file_handle);
    cr_assert_str_eq(file_contents, "This is a test request body.", "File contents do not match request body");

    // Check that the response is correct
    char expected_response[] = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\n";
    cr_assert_str_eq(response_buffer, expected_response, "Response is incorrect");

    // Clean up
    fclose(file_handle);
    remove("test_files/test.txt");
}

// Test creating a new file without a request body
Test(http_post, create_new_file_without_request_body)
{
    // Redirect stdout to a buffer to capture the response
    char response_buffer[RESPONSE_BUFFER_SIZE];
    cr_redirect_stdout_to_buffer(response_buffer, RESPONSE_BUFFER_SIZE);

    // Create a test request
    HttpRequest request = {
        .body = NULL,
        .request_line = {
            .method = "POST",
            .requestTarget = "/test.txt",
            .httpVersion = "HTTP/1.1"
        }
    };

    // Call the http_post function
    http_post(&request, 1);

    // Check that the file was created
    FILE *file_handle = fopen("test_files/test.txt", "r");
    cr_assert_not_null(file_handle, "File was not created");

    // Check that the file contents are empty
    char file_contents[BUFFER_SIZE];
    fgets(file_contents, BUFFER_SIZE, file_handle);
    cr_assert_str_empty(file_contents, "File contents are not empty");

    // Check that the response is correct
    char expected_response[] = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\n";
    cr_assert_str_eq(response_buffer, expected_response, "Response is incorrect");

    // Clean up
    fclose(file_handle);
    remove("test_files/test.txt");
}

// Test creating a new file with an invalid file extension
Test(http_post, create_new_file_with_invalid_file_extension)
{
    // Redirect stdout to a buffer to capture the response
    char response_buffer[RESPONSE_BUFFER_SIZE];
    cr_redirect_stdout_to_buffer(response_buffer, RESPONSE_BUFFER_SIZE);

    // Create a test request
    HttpRequest request = {
        .body = "This is a test request body.",
        .request_line = {
            .method = "POST",
            .requestTarget = "/test.xyz",
            .httpVersion = "HTTP/1.1"
        }
    };

    // Call the http_post function
    http_post(&request, 1);

    // Check that the file was not created
    FILE *file_handle = fopen("test_files/test.xyz", "r");
    cr_assert_null(file_handle, "File was created with invalid file extension");

    // Check that the response is correct
    char expected_response[] = "HTTP/1.1 415 Unsupported Media Type\r\nContent-Type: text/plain\r\n\r\n";
    cr_assert_str_eq(response_buffer, expected_response, "Response is incorrect");

    // Clean up
    remove("test_files/test.xyz");
}

// Test creating a new file with an existing file name
Test(http_post, create_new_file_with_existing_file_name)
{
    // Redirect stdout to a buffer to capture the response
    char response_buffer[RESPONSE_BUFFER_SIZE];
    cr_redirect_stdout_to_buffer(response_buffer, RESPONSE_BUFFER_SIZE);

    // Create a test file
    FILE *file_handle = fopen("test_files/test.txt", "w");
    fprintf(file_handle, "This is a test file.");
    fclose(file_handle);

    // Create a test request
    HttpRequest request = {
        .body = "This is a test request body.",
        .request_line = {
            .method = "POST",
            .requestTarget = "/test.txt",
            .httpVersion = "HTTP/1.1"
        }
    };

    // Call the http_post function
    http_post(&request, 1);

    // Check that the file was not overwritten
    file_handle = fopen("test_files/test.txt", "r");
    char file_contents[BUFFER_SIZE];
    fgets(file_contents, BUFFER_SIZE, file_handle);
    cr_assert_str_eq(file_contents, "This is a test file.", "File was overwritten");

    // Check that the response is correct
    char expected_response[] = "HTTP/1.1 409 Conflict\r\nContent-Type: text/plain\r\n\r\n";
    cr_assert_str_eq(response_buffer, expected_response, "Response is incorrect");

    // Clean up
    fclose(file_handle);
    remove("test_files/test.txt");
}

// Test creating a new file with a missing file name
Test(http_post, create_new_file_with_missing_file_name)
{
    // Redirect stdout to a buffer to capture the response
    char response_buffer[RESPONSE_BUFFER_SIZE];
    cr_redirect_stdout_to_buffer(response_buffer, RESPONSE_BUFFER_SIZE);

    // Create a test request
    HttpRequest request = {
        .body = "This is a test request body.",
        .request_line = {
            .method = "POST",
            .requestTarget = "/",
            .httpVersion = "HTTP/1.1"
        }
    };

    // Call the http_post function
    http_post(&request, 1);

    // Check that the file was not created
    FILE *file_handle = fopen("test_files/", "r");
    cr_assert_null(file_handle, "File was created with missing file name");

    // Check that the response is correct
    char expected_response[] = "HTTP/1.1 404 Not Found\r\nContent-Type: text/plain\r\n\r\n";
    cr_assert_str_eq(response_buffer, expected_response, "Response is incorrect");

    // Clean up
    remove("test_files/");
}