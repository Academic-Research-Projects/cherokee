#include "http_parser.h"

bool parse_http_request(int client_socket, struct HttpRequest *httpRequest)
{
    char buffer[1024];
    ssize_t bytes_received = recv(client_socket, buffer, sizeof(buffer) - 1, 0);

    if (bytes_received <= 0)
    {
        perror("recv");
        return false;
    }

    buffer[bytes_received] = '\0';

    char *token;

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

    httpRequest = > request_line.httpVersion = strdup(token);

    return true;
}