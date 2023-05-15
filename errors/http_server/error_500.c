#include "error_500.h"

char *internalServerErrorMessage(void)
{
    return "Internal Server Error";
}

HTTPError *createError500(void)
{
    HTTPError *error = malloc(sizeof(HTTPError));
    error->code = 500;
    error->message = internalServerErrorMessage;
    return error;
}