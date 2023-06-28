#ifndef HTTP_HANDLER_H
#define HTTP_HANDLER_H

#include "http/http_request/http_request.h"
#include "http/http_response/http_response.h"

void *handle_request(HttpRequest *request, int client_socket);

#endif