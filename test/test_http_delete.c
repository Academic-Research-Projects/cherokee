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
#include "crud_operations/http_delete.h"
#include "status_codes/http_status_codes.h"
#include "http/http_parser/http_parser.h"
#include "http/http_response/http_response.h"
#include "master/server_socket.h"

Test(http_delete, invalid_url)
{
    HttpRequest request = {
        .request_line = {
            .requestTarget = "invalid_url"
        }
    };
    char *response = http_delete(&request, 0);
    cr_assert_null(response, "Expected NULL response for invalid URL");
}

Test(createSuccess200ForDelete, success)
{
    // Create a new HttpResponse
    HttpResponse *response = malloc(sizeof(HttpResponse));

    // Call the createSuccess200 function
    createSuccess200(response, "text/plain");

    // Check that the response has the correct HTTP version, status code, status text, content type, and body
    cr_assert_str_eq(response->httpVersion, "HTTP/1.1", "Expected HTTP/1.1 version");
    cr_assert_eq(response->statusCode, 200, "Expected 200 status code");
    cr_assert_str_eq(response->statusText, "OK\n", "Expected 'OK' status message");
    cr_assert_str_eq(response->headers->name, "Content-Type", "Expected 'Content-Type' header name");
    cr_assert_str_eq(response->headers->value, "text/plain", "Expected 'text/plain' content type");
    cr_assert_str_eq(response->body, "OK\n", "Expected 'OK' response body");

    // Clean up any dynamically allocated resources
    free(response->headers);
    free(response->body);
    free(response);
}

Test(createError404ForDelete, not_found)
{
    // Create a new HttpResponse
    HttpResponse *response = malloc(sizeof(HttpResponse));

    // Call the createError404 function
    createError404(response);

    // Check that the response has the correct HTTP version, status code, status text, content type, and body
    cr_assert_str_eq(response->httpVersion, "HTTP/1.1", "Expected HTTP/1.1 version");
    cr_assert_eq(response->statusCode, 404, "Expected 404 status code");
    cr_assert_str_eq(response->statusText, "Not Found\n", "Expected 'Not Found' status message");
    cr_assert_str_eq(response->headers->name, "Content-Type", "Expected 'Content-Type' header name");
    cr_assert_str_eq(response->headers->value, "text/plain", "Expected 'text/plain' content type");
    cr_assert_str_eq(response->body, "Not Found\n", "Expected 'Not Found' response body");

    // Clean up any dynamically allocated resources
    free(response->headers);
    free(response->body);
    free(response);
}