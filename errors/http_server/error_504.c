#include "error_504.h"

char *gatewayTimeoutMessage(void)
{
    return "Gateway Timeout";
}

HTTPError *createError504(void)
{
    HTTPError *error = malloc(sizeof(HTTPError));
    error->code = 504;
    error->message = gatewayTimeoutMessage;
    return error;
}