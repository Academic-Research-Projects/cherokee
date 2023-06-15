#include "error_400.h"
#include "../../include/http/http_response/http_response.h"

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