#include "error_503.h"

char *serviceUnavailableMessage(void)
{
    return "Service Unavailable";
}

HTTPError *createError503(void)
{
    HTTPError *error = malloc(sizeof(HTTPError));
    error->code = 503;
    error->message = serviceUnavailableMessage;
    return error;
}