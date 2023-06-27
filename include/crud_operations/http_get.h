#ifndef HTTP_GET_H
#define HTTP_GET_H

#include "http/http_request/http_request.h"

void *http_get(HttpRequest *request, int client_socket);

#endif