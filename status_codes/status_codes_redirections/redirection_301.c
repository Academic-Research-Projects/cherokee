#include "status_codes/status_codes_redirections/redirection_301.h"
#include "http/http_response/http_response.h"
#include <stddef.h>

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
