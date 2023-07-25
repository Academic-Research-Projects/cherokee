#include "status_codes/http_status_codes.h"
#include "http/http_response/http_response.h"
#include <stddef.h>
#include <string.h>
#include <stdlib.h>

HttpResponse *createSuccess200(HttpResponse *response, char *content_type)
{
    response->headers = malloc(sizeof(struct ResponseHeaders));
    response->httpVersion = "HTTP/1.1";
    response->statusCode = 200;
    response->statusText = "OK\n";
    response->headers->name = "Content-Type";
    response->headers->value = content_type;
    response->body = strdup("OK\n");
    return response;
}