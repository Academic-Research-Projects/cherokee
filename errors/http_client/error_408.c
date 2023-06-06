#include "error_408.h"

char *requestTimeoutMessage(void)
{
    return "Request Timeout";
}

HttpResponse *createError408(void)
{
    HttpResponse *error = malloc(sizeof(HttpResponse));
    error->code = 408;
    error->message = requestTimeoutMessage;
    return error;
}