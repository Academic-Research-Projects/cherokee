#include "error_403.h"

char *forbiddenMessage(void)
{
    return "Forbidden";
}

HttpResponse *createError403(void)
{
    HttpResponse *error = malloc(sizeof(HttpResponse));
    error->code = 403;
    error->message = forbiddenMessage;
    return error;
}