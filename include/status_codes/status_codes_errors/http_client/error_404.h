#ifndef ERROR_404_H
#define ERROR_404_H

#include "http/http_response/http_response.h"

HttpResponse *createNotFoundError(HttpResponse *response);

#endif