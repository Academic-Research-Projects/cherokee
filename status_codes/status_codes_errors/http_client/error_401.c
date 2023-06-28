#include "status_codes/status_codes_errors/http_client/error_401.h"
#include "http/http_response/http_response.h"
#include <stddef.h>

char *unauthorizedMessage(void)
{
    return "Unauthorized";
}

HttpResponse *createError401(HttpResponse *response)
{
    response->httpVersion = strdup("HTTP/1.1");
    response->statusCode = 401;
    response->statusText = unauthorizedMessage();
    response->headers = NULL;
    response->body = NULL;
    return response;
}