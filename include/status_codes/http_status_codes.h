#ifndef HTTP_STATUS_CODES_H
#define HTTP_STATUS_CODES_H

#include "http/http_response/http_response.h"

typedef enum
{
    HTTP_OK = 200,
    HTTP_CREATED = 201,
    HTTP_NO_CONTENT = 204,
    HTTP_BAD_REQUEST = 400,
    HTTP_UNAUTHORIZED = 401,
    HTTP_FORBIDDEN = 403,
    HTTP_NOT_FOUND = 404,
    HTTP_METHOD_NOT_ALLOWED = 405,
    HTTP_CONFLICT = 409,
    HTTP_TOO_MANY_REQUESTS = 429,
    HTTP_INTERNAL_SERVER_ERROR = 500
} HttpStatusCode;

// Client Error Handling
HttpResponse *createError404(HttpResponse *response);
HttpResponse *createError400(HttpResponse *response);
HttpResponse *createError401(HttpResponse *response);
HttpResponse *createError403(HttpResponse *response);
HttpResponse *createError405(HttpResponse *response);
HttpResponse *createError409(HttpResponse *response);

// Server Error Handling
HttpResponse *createError500(HttpResponse *response);

// Success Handling
HttpResponse *createSuccess200(HttpResponse *response, char *content_type);
HttpResponse *createSuccess201(HttpResponse *response);
HttpResponse *createSuccess204(HttpResponse *response); //, char *content_type);

#endif