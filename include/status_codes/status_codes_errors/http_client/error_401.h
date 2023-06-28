#ifndef ERROR_401_H
#define ERROR_401_H

#include "http/http_response/http_response.h"

HttpResponse *createUnauthorizedError(HttpResponse *response);

#endif