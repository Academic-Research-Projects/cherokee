#ifndef ERROR_503_H
#define ERROR_503_H

#include "http_response.h"

HttpResponse *createServiceUnavailableError(HttpResponse *response);

#endif