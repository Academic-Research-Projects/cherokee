#include "http/http_formatter/http_formatter.h"
#include "http/http_response/http_response.h"
#include <stdio.h>
#include <string.h>

char *format_http_response(HttpResponse *response)
{
    char buffer[512];

    if (response->body != NULL)
    {
        sprintf(buffer, "%s %d %s %s", response->httpVersion, response->statusCode, response->statusText, response->body);
    }
    else
    {
        sprintf(buffer, "%s %d %s", response->httpVersion, response->statusCode, response->statusText);
    }

    char *response_str = strdup(buffer);
    return response_str;
}