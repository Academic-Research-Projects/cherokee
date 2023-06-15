#include "error_505.h"
#include "../../include/http/http_response/http_response.h"

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