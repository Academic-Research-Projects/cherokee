#include "error_502.h"

char *badGatewayMessage(void)
{
    return "Bad Gateway";
}

HTTPError *createError502(void)
{
    HTTPError *error = malloc(sizeof(HTTPError));
    error->code = 502;
    error->message = badGatewayMessage;
    return error;
}