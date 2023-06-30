#include <criterion/criterion.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>

#include "master/server_socket.h"

Test(test_create_socket, returns_valid_socket_descriptor)
{
    int socket_fd = create_socket();

    // test if socket descriptor is valid
    cr_assert(socket_fd > 0, "Invalid socket descriptor");

    close(socket_fd);
}

Test(test_bind_socket, binds_to_correct_port)
{
    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd < 0)
    {
        exit(1);
    }

    int port = 8080;
    bind_socket(socket_fd, port);

    struct sockaddr_in server_address;
    socklen_t server_address_len = sizeof(server_address);
    if (getsockname(socket_fd, (struct sockaddr *)&server_address, &server_address_len) < 0)
    {
        exit(1);
    }

    // test if socket is bound to correct port
    cr_assert(ntohs(server_address.sin_port) == port, "Socket did not bind to correct port");

    close(socket_fd);
}

Test(test_listen_on_socket, socket_is_on_listen_mode)
{
    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd < 0)
    {
        exit(1);
    }

    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(8080);
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(socket_fd, (struct sockaddr *)&server_address, sizeof(server_address)) < 0)
    {
        exit(1);
    }

    listen_on_socket(socket_fd);

    // test if socket is in listen mode
    int socket_flags = fcntl(socket_fd, F_GETFL, 0);
    cr_assert(socket_flags > 0, "Error accepting connection");

    close(socket_fd);
}