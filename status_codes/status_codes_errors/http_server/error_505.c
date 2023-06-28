#include "status_codes/status_codes_errors/http_server/error_505.h"
#include "http/http_response/http_response.h"
#include <stddef.h>

char *httpVersionNotSupportedMessage(void)
{
    return "HTTP Version Not Supported";
}

HttpResponse *createError505(HttpResponse *response)
{
    response->httpVersion = strdup("HTTP/1.1");
    response->statusCode = 505;
    response->statusText = httpVersionNotSupportedMessage();
    response->headers = NULL;
    response->body = NULL;
    return response;
}