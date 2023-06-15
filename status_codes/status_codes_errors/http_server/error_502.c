#include "error_502.h"
#include "../../include/http/http_response/http_response.h"

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