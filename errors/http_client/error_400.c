#include "error_400.h"

char *badRequestMessage(void)
{
    return "Bad Request";
}

HTTPError *createError400(void)
{
    HTTPError *error = malloc(sizeof(HTTPError));
    error->code = 400;
    error->message = badRequestMessage;
    return error;
}