#include "error_400.h"

char *badRequestMessage(void)
{
    return "Bad Request";
}

HttpResponse *createError400(void)
{
    HttpResponse *error = malloc(sizeof(HttpResponse));
    error->code = 400;
    error->message = badRequestMessage;
    return error;
}