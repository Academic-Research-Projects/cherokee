#include "status_codes/http_status_codes.h"
#include "http/http_response/http_response.h"
#include <stddef.h>
#include <string.h>
#include <stdlib.h>

char *unauthorizedMessage(void)
{
    return "Unauthorized";
}

HttpResponse *createError401(HttpResponse *response)
{
    response->headers = malloc(sizeof(struct ResponseHeaders));
    response->headers->name = "Content-Type";
    response->headers->value = "text/plain";
    response->httpVersion = "HTTP/1.1";
    response->statusCode = 401;
    response->statusText = unauthorizedMessage();
    response->body = strdup(unauthorizedMessage());
    return response;
}