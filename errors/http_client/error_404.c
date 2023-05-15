#include "error_404.h"

char *notFoundMessage(void)
{
    return "Not Found";
}

HTTPError *createError404(void)
{
    HTTPError *error = malloc(sizeof(HTTPError));
    error->code = 404;
    error->message = notFoundMessage;
    return error;
}