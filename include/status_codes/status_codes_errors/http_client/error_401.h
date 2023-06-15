#ifndef ERROR_401_H
#define ERROR_401_H

#include "http_response.h"

HttpResponse *createUnauthorizedError(HttpResponse *response);

#endif