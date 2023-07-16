#include "status_codes/http_status_codes.h"
#include "http/http_response/http_response.h"
#include <stddef.h>
#include <string.h>
#include <stdlib.h>

char *noContentMessage(void)
{
    return "No Content\n";
}

HttpResponse *createSuccess204(HttpResponse *response) //, char *content_type)
{
    response->headers = malloc(sizeof(struct ResponseHeaders));
    response->headers->name = "Content-Type";
    response->headers->value = "text/plain";
    response->httpVersion = "HTTP/1.1";
    response->statusCode = 204;
    response->statusText = noContentMessage();
    response->body = strdup(noContentMessage());
    // response->httpVersion = strdup("HTTP/1.1");
    // response->statusCode = 204;
    // response->statusText = noContentMessage();
    // response->headers->name = "Content-Type";
    // response->headers->value = content_type;
    // response->headers = NULL;
    // response->body = NULL;
    return response;
}