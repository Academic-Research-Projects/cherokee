#ifndef SUCCESS_200_H
#define SUCCESS_200_H

#include "http/http_response/http_response.h"

// HttpResponse *createSuccess200(HttpResponse *response);
HttpResponse *createSuccess200(HttpResponse *response, char *content_type);
// HttpResponse *createSuccess200(HttpResponse *response);

#endif
