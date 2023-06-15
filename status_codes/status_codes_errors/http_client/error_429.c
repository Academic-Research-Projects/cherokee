#include "error_429.h"
#include "../../include/http/http_response/http_response.h"

char *tooManyRequestsMessage(void)
{
    return "Too Many Requests";
}

HttpResponse *createError429(HttpResponse *response)
{
    response->httpVersion = strdup("HTTP/1.1");
    response->statusCode = 429;
    response->statusText = tooManyRequestsMessage();
    response->headers = NULL;
    response->body = NULL;
    return response;
}