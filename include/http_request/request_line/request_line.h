#ifndef REQUEST_LINE_H
#define REQUEST_LINE_H

typedef struct
{
    char *method;
    char *uri;
} RequestLine;

#endif