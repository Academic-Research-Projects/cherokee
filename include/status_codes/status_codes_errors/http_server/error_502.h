#ifndef ERROR_502_H
#define ERROR_502_H

#include "http/http_response/http_response.h"

HttpResponse *createBadGatewayError(HttpResponse *response);

#endif