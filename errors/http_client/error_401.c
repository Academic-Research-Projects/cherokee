#include "error_401.h"

char *unauthorizedMessage(void)
{
    return "Unauthorized";
}

HttpResponse *createError401(void)
{
    HttpResponse *error = malloc(sizeof(HttpResponse));
    error->code = 401;
    error->message = unauthorizedMessage;
    return error;
}