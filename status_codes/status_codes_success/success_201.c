#include "status_codes/http_status_codes.h"
#include "http/http_response/http_response.h"
#include <stddef.h>
#include <string.h>
#include <stdlib.h>

char *createdMessage(void)
{
    return "Created";
}

HttpResponse *createSuccess201(HttpResponse *response)
{
    response->httpVersion = "HTTP/1.1";
    response->statusCode = 201;
    response->statusText = createdMessage();
    response->headers = NULL;
    response->body = NULL;
    return response;
}