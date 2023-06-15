#include "error_200.h"

char *successMessage(void)
{
    return "OK";
}

HttpResponse *createSuccess200(HttpResponse *response)
{
    response->httpVersion = strdup("HTTP/1.1");
    response->statusCode = 200;
    response->statusText = successMessage();
    response->headers = NULL;
    response->body = NULL;
    return response;
}