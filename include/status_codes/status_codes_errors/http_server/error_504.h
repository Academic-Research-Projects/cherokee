#ifndef ERROR_504_H
#define ERROR_504_H

#include "http/http_response/http_response.h"

HttpResponse *createGatewayTimeoutError(HttpResponse *response);

#endif