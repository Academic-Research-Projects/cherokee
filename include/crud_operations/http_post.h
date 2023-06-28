#ifndef HTTP_POST_H
#define HTTP_POST_H
#include "http/http_request/http_request.h"

void *http_post(HttpRequest *request, int client_socket);

#endif