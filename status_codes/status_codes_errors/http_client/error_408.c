#include "error_408.h"
#include "../../include/http/http_response/http_response.h"

char *requestTimeoutMessage(void)
{
    return "Request Timeout";
}

HttpResponse *createError408(HttpResponse *response)
{
    response->httpVersion = strdup("HTTP/1.1");
    response->statusCode = 408;
    response->statusText = requestTimeoutMessage();
    response->headers = NULL;
    response->body = NULL;
    return response;
}