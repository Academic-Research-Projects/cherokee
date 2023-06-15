#ifndef ERROR_502_H
#define ERROR_502_H

#include "http_response.h"

HttpResponse *createBadGatewayError(HttpResponse *response);

#endif