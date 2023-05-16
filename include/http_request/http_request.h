#ifndef HTTP_REQUEST_H
#define HTTP_REQUEST_H

#include "request_line.h"
#include "headers.h"

typedef struct
{
    RequestLine request_line;
    Headers headers;
} HttpRequest;

#endif