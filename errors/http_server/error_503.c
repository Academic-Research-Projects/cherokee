#include "error_503.h"

char *serviceUnavailableMessage(void)
{
    return "Service Unavailable";
}

HttpResponse *createError503(void)
{
    HttpResponse *error = malloc(sizeof(HttpResponse));
    error->code = 503;
    error->message = serviceUnavailableMessage;
    return error;
}