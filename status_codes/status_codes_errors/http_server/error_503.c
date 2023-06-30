#include "status_codes/status_codes_errors/http_server/error_503.h"
#include "http/http_response/http_response.h"
#include <stddef.h>

char *serviceUnavailableMessage(void)
{
    return "Service Unavailable";
}

HttpResponse *createError503(HttpResponse *response)
{
    response->httpVersion = strdup("HTTP/1.1");
    response->statusCode = 503;
    response->statusText = serviceUnavailableMessage();
    response->headers = NULL;
    response->body = NULL;
    return response;
}