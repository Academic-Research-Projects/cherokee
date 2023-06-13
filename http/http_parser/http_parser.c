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

    httpRequest->request_line.method = strtok(buffer, " ");
}