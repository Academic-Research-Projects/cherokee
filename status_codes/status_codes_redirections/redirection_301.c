#include "redirection_301.h"

char *movedPermanentlyMessage(void)
{
    return "Moved Permanently";
}

HttpResponse *createRedirection301(HttpResponse *response)
{
    response->httpVersion = strdup("HTTP/1.1");
    response->statusCode = 301;
    response->statusText = movedPermanentlyMessage();
    response->headers = NULL;
    response->body = NULL;
    return response;
}
