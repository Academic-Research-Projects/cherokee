/**
 * The function `parse_http_request` reads and parses an HTTP request from a client socket.
 * 
 * @param client_socket The `client_socket` parameter is the file descriptor of the socket that is
 * connected to the client. It is used to receive data from the client.
 * @param httpRequest The `httpRequest` parameter is a pointer to a struct of type `HttpRequest`. This
 * struct contains information about the HTTP request, such as the method, request target, and HTTP
 * version.
 * 
 * @return The function `parse_http_request` returns a boolean value. It returns `true` if the HTTP
 * request is successfully parsed and stored in the `HttpRequest` structure, and `false` otherwise.
 */
#include <stdbool.h>
#include <stddef.h>
#include "http/http_request/http_request.h"
#include <sys/socket.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

bool parse_http_request(int client_socket, HttpRequest *httpRequest)
{
    char buffer[1024] = {0};
    char *token;

    size_t bytes_received = recv(client_socket, buffer, sizeof(buffer) - 1, 0);

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