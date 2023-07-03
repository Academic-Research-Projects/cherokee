#include "status_codes/http_status_codes.h"
#include "http/http_response/http_response.h"
#include <stddef.h>
#include <string.h>
#include <stdlib.h>

char *conflictMessage(void)
{
    return "Conflict";
}

HttpResponse *createError409(HttpResponse *response)
{
    response->headers = malloc(sizeof(struct ResponseHeaders));
    response->headers->name = "Content-Type";
    response->headers->value = "text/plain";
    response->httpVersion = "HTTP/1.1";
    response->statusCode = 409;
    response->statusText = conflictMessage();
    response->body = strdup(conflictMessage());
    return response;
}