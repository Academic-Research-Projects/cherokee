#include "error_401.h"

char *unauthorizedMessage(void)
{
    return "Unauthorized";
}

HTTPError *createError401(void)
{
    HTTPError *error = malloc(sizeof(HTTPError));
    error->code = 401;
    error->message = unauthorizedMessage;
    return error;
}