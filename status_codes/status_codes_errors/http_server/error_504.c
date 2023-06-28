#include "status_codes/status_codes_errors/http_server/error_504.h"
#include "http/http_response/http_response.h"
#include <stddef.h>

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
