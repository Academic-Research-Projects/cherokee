#ifndef ERROR_429_H
#define ERROR_429_H

#include "http/http_response/http_response.h"

HttpResponse *createTooManyRequestsError(HttpResponse *response);

#endif