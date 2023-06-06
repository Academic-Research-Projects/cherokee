#include "error_505.h"

char *httpVersionNotSupportedMessage(void)
{
    return "HTTP Version Not Supported";
}

HttpResponse *createError505(void)
{
    HttpResponse *error = malloc(sizeof(HttpResponse));
    error->code = 505;
    error->message = httpVersionNotSupportedMessage;
    return error;
}