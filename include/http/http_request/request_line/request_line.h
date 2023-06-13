#ifndef REQUEST_LINE_H
#define REQUEST_LINE_H

typedef struct
{
    char *method;
    char *requestTarget;
    char *httpVersion; // 1.1
} RequestLine;

#endif