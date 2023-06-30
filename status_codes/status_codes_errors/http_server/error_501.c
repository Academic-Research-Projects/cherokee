#include "status_codes/status_codes_errors/http_server/error_501.h"
#include "http/http_response/http_response.h"
#include <stddef.h>

char *notImplementedMessage(void)
{
    return "Not Implemented";
}

HttpResponse *createError501(HttpResponse *response)
{
    response->httpVersion = strdup("HTTP/1.1");
    response->statusCode = 501;
    response->statusText = notImplementedMessage();
    response->headers = NULL;
    response->body = NULL;
    return response;
}