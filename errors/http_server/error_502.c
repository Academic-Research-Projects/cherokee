#include "error_502.h"

char *badGatewayMessage(void)
{
    return "Bad Gateway";
}

HttpResponse *createError502(void)
{
    HttpResponse *error = malloc(sizeof(HttpResponse));
    error->code = 502;
    error->message = badGatewayMessage;
    return error;
}