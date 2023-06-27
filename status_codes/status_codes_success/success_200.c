#include "status_codes/stauts_codes_success/success_200.h"
#include "http/http_response/http_response.h"
#include <stddef.h>
#include <string.h>
#include <stdlib.h>

char *successMessage(void)
{
    return "OK";
}

HttpResponse *createSuccess200(HttpResponse *response, char *content_type)
//HttpResponse *createSuccess200(HttpResponse *response)
{
    //allocating memory
    response->headers = malloc(sizeof(struct ResponseHeaders));

    response->httpVersion = strdup("HTTP/1.1");
    response->statusCode = 200;
    response->statusText = successMessage();
    response->headers->name = "Content-Type";
    response->headers->value = content_type;
    response->body = NULL;
    return response;
}