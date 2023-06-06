#include "error_429.h"

char *tooManyRequestsMessage(void)
{
    return "Too Many Requests";
}

HttpResponse *createError429(void)
{
    HttpResponse *error = malloc(sizeof(HttpResponse));
    error->code = 429;
    error->message = tooManyRequestsMessage;
    return error;
}