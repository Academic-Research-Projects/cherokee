#ifndef HTTP_RESPONSE_H
#define HTTP_RESPONSE_H

typedef struct
{
    int code;
    char *(*message)(void);
} HttpResponse;

#endif