#ifndef ERROR_500_H
#define ERROR_500_H

#include "http/http_response/http_response.h"

HttpResponse *createInternalServerError(HttpResponse *response);

#endif