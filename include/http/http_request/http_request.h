#ifndef HTTP_REQUEST_H
#define HTTP_REQUEST_H

#include "http/http_request/request_line/request_line.h"
#include "http/http_request/headers/headers.h"

typedef struct HttpRequest
{
    RequestLine request_line;
    Headers headers;
    char *body; // For methods like POST, PUT etc...
} HttpRequest;

#endif