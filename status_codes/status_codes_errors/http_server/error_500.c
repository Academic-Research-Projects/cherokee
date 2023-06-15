#include "error_500.h"
#include "../../include/http/http_response/http_response.h"

char *internalServerErrorMessage(void)
{
    return "Internal Server Error";
}

HttpResponse *createError500(HttpResponse *response)
{
    response->httpVersion = strdup("HTTP/1.1");
    response->statusCode = 500;
    response->statusText = internalServerErrorMessage();
    response->headers = NULL;
    response->body = NULL;
    return response;
}