/**
 * The function `format_http_response` takes a `HttpResponse` struct as input and returns a formatted
 * string representation of the HTTP response.
 *
 * @param response The `response` parameter is a pointer to a `HttpResponse` struct.
 *
 * @return The function `format_http_response` returns a dynamically allocated string that represents
 * the formatted HTTP response.
 */
#include "http/http_formatter/http_formatter.h"
#include "http/http_response/http_response.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char *format_http_response(HttpResponse *response)
{
    char buffer[1024]; // Increase the buffer size if needed

    // Include the headers in the formatted string
    if (response->body != NULL)
    {
        snprintf(buffer, sizeof(buffer), "%s %d %s\r\n%s: %s\r\n\r\n%s",
                 response->httpVersion, response->statusCode, response->statusText,
                 response->headers->name, response->headers->value,
                 response->body);
    }
    else
    {
        snprintf(buffer, sizeof(buffer), "%s %d %s\r\n%s: %s\r\n\r\n",
                 response->httpVersion, response->statusCode, response->statusText,
                 response->headers->name, response->headers->value);
    }

    char *response_str = strdup(buffer);
    return response_str;
}
