#include "status_codes/http_status_codes.h"
#include "http/http_response/http_response.h"
#include <stddef.h>
#include <string.h>
#include <stdlib.h>

char *noContentMessage(void)
{
    return "No Content\n";
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