#ifndef HTTP_ERROR_H
#define HTTP_ERROR_H

typedef struct
{
    int code;
    char *(*message)(void);
} HTTPError;

#endif