#include "error_401.h"
#include "../../include/http/http_response/http_response.h"

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