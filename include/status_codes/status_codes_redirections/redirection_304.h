#ifndef REDIRECTION_304_H
#define REDIRECTION_304_H

#include "http/http_response/http_response.h"

HttpResponse *createNotModifiedRedirection(HttpResponse *response);

#endif