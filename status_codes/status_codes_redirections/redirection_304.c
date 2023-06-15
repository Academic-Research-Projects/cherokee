#include "redirection_304.h"

char *notModifiedMessage(void)
{
    return "Not Modified";
}

HttpResponse *createRedirection304(HttpResponse *response)
{
    response->httpVersion = strdup("HTTP/1.1");
    response->statusCode = 304;
    response->statusText = notModifiedMessage();
    response->headers = NULL;
    response->body = NULL;
    return response;
}