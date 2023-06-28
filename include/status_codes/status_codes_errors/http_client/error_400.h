#ifndef ERROR_400_H
#define ERROR_400_H

#include "http/http_response/http_response.h"

HttpResponse *createBadRequestError(HttpResponse *response);

#endif
