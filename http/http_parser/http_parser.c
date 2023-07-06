#include <stdbool.h>
#include <stddef.h>
#include "http/http_request/http_request.h"
#include <sys/socket.h>
#include <stdio.h>
#include <string.h>

bool parse_http_request(int client_socket, HttpRequest *httpRequest)
{
    char buffer[1024];
    char *token;

    size_t bytes_received = recv(client_socket, buffer, sizeof(buffer) - 1, 0);

    printf(buffer);

    if (bytes_received <= 0)
    {
        perror("recv");
        return false;
    }

    buffer[bytes_received] = '\0';

    char *body = strstr(buffer, "\r\n\r\n");

    if (body != NULL)
        httpRequest->body = strdup(body + 4);
    else
        httpRequest->body = NULL;

    token = strtok(buffer, " ");
    if (token == NULL)
    {
        perror("strtok");
        return false;
    }

    httpRequest->request_line.method = strdup(token);

    token = strtok(NULL, " ");
    if (token == NULL)
    {
        perror("strtok");
        return false;
    }

    httpRequest->request_line.requestTarget = strdup(token);

    token = strtok(NULL, "\r\n");
    if (token == NULL)
    {
        perror("strtok");
        return false;
    }

    httpRequest->request_line.httpVersion = strdup(token);

    return true;
}