#include "error_501.h"

char *notImplementedMessage(void)
{
    return "Not Implemented";
}

HttpResponse *createError501(void)
{
    HttpResponse *error = malloc(sizeof(HttpResponse));
    error->code = 501;
    error->message = notImplementedMessage;
    return error;
}