#ifndef ERROR_505_H
#define ERROR_505_H

#include "http_response.h"

HttpResponse *createHTTPVersionNotSupportedError(HttpResponse *response);

#endif