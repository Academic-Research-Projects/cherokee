#include "http/http_formatter/http_formatter.h"
#include "http/http_response/http_response.h"
#include <stdio.h>
#include <string.h>

char *format_http_response(HttpResponse *response)
{
    char buffer[1024];  // Increase the buffer size if needed

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
