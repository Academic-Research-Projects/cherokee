#include "error_504.h"

char *gatewayTimeoutMessage(void)
{
    return "Gateway Timeout";
}

HttpResponse *createError504(void)
{
    HttpResponse *error = malloc(sizeof(HttpResponse));
    error->code = 504;
    error->message = gatewayTimeoutMessage;
    return error;
}