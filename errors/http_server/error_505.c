#include "error_505.h"

char *httpVersionNotSupportedMessage(void)
{
    return "HTTP Version Not Supported";
}

HTTPError *createError505(void)
{
    HTTPError *error = malloc(sizeof(HTTPError));
    error->code = 505;
    error->message = httpVersionNotSupportedMessage;
    return error;
}