#ifndef REDIRECTION_301_H
#define REDIRECTION_301_H

#include "http/http_response/http_response.h"

HttpResponse *createMovedPermanentlyRedirection(HttpResponse *response);

#endif