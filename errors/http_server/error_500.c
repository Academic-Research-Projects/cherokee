#include "error_500.h"

char *internalServerErrorMessage(void)
{
    return "Internal Server Error";
}

HttpResponse *createError500(void)
{
    HttpResponse *error = malloc(sizeof(HttpResponse));
    error->code = 500;
    error->message = internalServerErrorMessage;
    return error;
}