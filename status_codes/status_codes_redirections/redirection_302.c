#include "redirection_302.h"

char *foundMessage(void)
{
    return "Found";
}

HttpResponse *createRedirection302(HttpResponse *response)
{
    response->httpVersion = strdup("HTTP/1.1");
    response->statusCode = 302;
    response->statusText = foundMessage();
    response->headers = NULL;
    response->body = NULL;
    return response;
}