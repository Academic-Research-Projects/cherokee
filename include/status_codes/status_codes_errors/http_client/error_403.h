#ifndef ERROR_403_H
#define ERROR_403_H

#include "http_response.h"

HttpResponse *createForbiddenError(HttpResponse *response);

#endif