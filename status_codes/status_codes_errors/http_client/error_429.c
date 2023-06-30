#include "status_codes/status_codes_errors/http_client/error_429.h"
#include "http/http_response/http_response.h"
#include <stddef.h>

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