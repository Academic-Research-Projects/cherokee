#include "success_204.h"

char *noContentMessage(void)
{
    return "No Content";
}

HttpResponse *createSuccess204(HttpResponse *response)
{
    response->httpVersion = strdup("HTTP/1.1");
    response->statusCode = 204;
    response->statusText = noContentMessage();
    response->headers = NULL;
    response->body = NULL;
    return response;
}