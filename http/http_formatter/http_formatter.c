#include "http_response.h"

format_http_response(HttpResponse *response)
{
    char buffer[512];

    if (response->body != null)
    {
        sprintf(buffer, response->httpVersion, response->statusCode, response->statusText, response->body);
    }
    else
    {
        sprintf(buffer, response->httpVersion, response->statusCode, response->statusText);
    }

    char *response_str = strdup(buffer);
    return response_str;
}