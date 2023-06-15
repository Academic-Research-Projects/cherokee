#ifndef HTTP_RESPONSE_H
#define HTTP_RESPONSE_H

struct ResponseHeaders
{
    char *name;
    char *value;
    // struct ResponseHeaders *next; //For multiple header scenario.
};

struct HttpResponse
{
    char *httpVersion;
    int statusCode;
    char *statusText;
    struct ResponseHeaders *headers;
    char *body;
};

#endif