#include "status_codes/status_codes_errors/http_client/error_404.h"
#include "http/http_response/http_response.h"
#include <stddef.h>
#include <string.h>
#include <stdlib.h>

char *notFoundMessage(void)
{
    return "Not Found";
}

HttpResponse *createError404(HttpResponse *response)
{
    response->headers = malloc(sizeof(struct ResponseHeaders));
    response->httpVersion = strdup("HTTP/1.1");
    response->statusCode = 404;
    response->statusText = notFoundMessage();
    response->headers->name = NULL;
    response->headers->value = NULL;
    response->body = NULL;
    return response;
}