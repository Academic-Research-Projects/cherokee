#ifndef HTTP_HEAD_H
#define HTTP_HEAD_H

#include "http/http_request/http_request.h"

void *http_head(HttpRequest *request, int client_socket);

#endif