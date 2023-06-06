#include "error_404.h"

char *notFoundMessage(void)
{
    return "Not Found";
}

HttpResponse *createError404(void)
{
    HttpResponse *error = malloc(sizeof(HttpResponse));
    error->code = 404;
    error->message = notFoundMessage;
    return error;
}