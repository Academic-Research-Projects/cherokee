#include "status_codes/status_codes_errors/http_client/error_400.h"
#include "http/http_response/http_response.h"
#include <stddef.h>

char *badRequestMessage(void)
{
    return "Bad Request";
}

HttpResponse *createError400(HttpResponse *response)
{
    response->httpVersion = strdup("HTTP/1.1");
    response->statusCode = 400;
    response->statusText = badRequestMessage();
    response->headers = NULL;
    response->body = NULL;
    return response;
}