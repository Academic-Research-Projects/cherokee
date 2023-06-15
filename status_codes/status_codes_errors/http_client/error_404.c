#include "error_404.h"
#include "../../include/http/http_response/http_response.h"

char *notFoundMessage(void)
{
    return "Not Found";
}

HttpResponse *createError404(HttpResponse *response)
{
    response->httpVersion = strdup("HTTP/1.1");
    response->statusCode = 404;
    response->statusText = notFoundMessage();
    response->headers = NULL;
    response->body = NULL;
    return response;
}