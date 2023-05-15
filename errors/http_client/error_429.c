#include "error_429.h"

char *tooManyRequestsMessage(void)
{
    return "Too Many Requests";
}

HTTPError *createError429(void)
{
    HTTPError *error = malloc(sizeof(HTTPError));
    error->code = 429;
    error->message = tooManyRequestsMessage;
    return error;
}