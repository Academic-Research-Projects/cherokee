#include "error_403.h"
#include "../../include/http/http_response/http_response.h"

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