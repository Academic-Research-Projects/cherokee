#ifndef ERROR_408_H
#define ERROR_408_H

#include "http/http_response/http_response.h"

HttpResponse *createRequestTimeoutError(HttpResponse *response);

#endif