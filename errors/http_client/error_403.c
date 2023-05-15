#include "error_403.h"

char *forbiddenMessage(void)
{
    return "Forbidden";
}

HTTPError *createError403(void)
{
    HTTPError *error = malloc(sizeof(HTTPError));
    error->code = 403;
    error->message = forbiddenMessage;
    return error;
}