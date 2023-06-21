#include "status_codes/status_codes_errors/http_server/error_502.h"
#include "http/http_response/http_response.h"
#include <stddef.h>

char *badGatewayMessage(void)
{
    return "Bad Gateway";
}

HttpResponse *createError502(HttpResponse *response)
{
    response->httpVersion = strdup("HTTP/1.1");
    response->statusCode = 502;
    response->statusText = badGatewayMessage();
    response->headers = NULL;
    response->body = NULL;
    return response;
}