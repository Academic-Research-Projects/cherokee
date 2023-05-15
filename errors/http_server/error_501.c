#include "error_501.h"

char *notImplementedMessage(void)
{
    return "Not Implemented";
}

HTTPError *createError501(void)
{
    HTTPError *error = malloc(sizeof(HTTPError));
    error->code = 501;
    error->message = notImplementedMessage;
    return error;
}