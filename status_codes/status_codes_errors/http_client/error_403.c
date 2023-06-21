#include "status_codes/status_codes_errors/http_client/error_403.h"
#include "http/http_response/http_response.h"
#include <stddef.h>

char *forbiddenMessage(void)
{
    return "Forbidden";
}

HttpResponse *createError403(HttpResponse *response)
{
    response->httpVersion = strdup("HTTP/1.1");
    response->statusCode = 403;
    response->statusText = forbiddenMessage();
    response->headers = NULL;
    response->body = NULL;
    return response;
}