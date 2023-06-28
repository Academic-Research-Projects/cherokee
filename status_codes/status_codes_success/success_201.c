#include "status_codes/stauts_codes_success/success_201.h"
#include "http/http_response/http_response.h"
#include <stddef.h>

char *createdMessage(void)
{
    return "Created";
}

HttpResponse *createSuccess201(HttpResponse *response)
{
    response->httpVersion = strdup("HTTP/1.1");
    response->statusCode = 201;
    response->statusText = createdMessage();
    response->headers = NULL;
    response->body = NULL;
    return response;
}