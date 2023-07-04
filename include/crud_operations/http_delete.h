#ifndef HTTP_DELETE_H
#define HTTP_DELETE_H

#include "http/http_request/http_request.h"

void *http_delete(HttpRequest *request, int client_socket);

#endif