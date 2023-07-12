#ifndef HTTP_PUT_H
#define HTTP_PUT_H

#include "http/http_request/http_request.h"

void *http_put(HttpRequest *request, int client_socket);

#endif