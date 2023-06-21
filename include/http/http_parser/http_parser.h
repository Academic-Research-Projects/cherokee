#ifndef HTTP_PARSER_H
#define HTTP_PARSER_H

#include <stdbool.h>

#include "http/http_request/http_request.h"

bool parse_http_request(int client_socket, HttpRequest *httpRequest);

#endif