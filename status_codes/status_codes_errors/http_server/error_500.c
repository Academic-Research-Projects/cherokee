#include "status_codes/http_status_codes.h"
#include "http/http_response/http_response.h"
#include <stddef.h>
#include <string.h>
#include <stdlib.h>

char *internalServerErrorMessage(void)
{
    return "Internal Server Error";
}

HttpResponse *createError500(HttpResponse *response)
{
    response->httpVersion = "HTTP/1.1";
    response->statusCode = 500;
    response->statusText = internalServerErrorMessage();
    response->headers = NULL;
    response->body = NULL;
    return response;
}