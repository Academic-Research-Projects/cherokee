#include "error_408.h"

char *requestTimeoutMessage(void)
{
    return "Request Timeout";
}

HTTPError *createError408(void)
{
    HTTPError *error = malloc(sizeof(HTTPError));
    error->code = 408;
    error->message = requestTimeoutMessage;
    return error;
}