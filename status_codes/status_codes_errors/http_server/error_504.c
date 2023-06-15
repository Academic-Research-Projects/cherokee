#include "error_504.h"
#include "../../include/http/http_response/http_response.h"

char *gatewayTimeoutMessage(void)
{
    return "Gateway Timeout";
}

HttpResponse *createError504(HttpResponse *response)
{
    response->httpVersion = strdup("HTTP/1.1");
    response->statusCode = 504;
    response->statusText = gatewayTimeoutMessage();
    response->headers = NULL;
    response->body = NULL;
    return response;
}
