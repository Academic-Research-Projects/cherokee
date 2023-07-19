#include <criterion/criterion.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "master/worker.h"

Test(test_create_epoll_instance, returns_valid_epoll_fd)
{
    int epoll_fd = create_epoll_instance();
    cr_assert(epoll_fd != -1, "epoll_fd should not be -1");
}

Test(test_add_server_socket_to_epoll, returns_correct_epoll_struct)
{
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1)
    {
        perror("socket\n");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in server_address;
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(8081);

    if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) == -1)
    {
        perror("bind\n");
        exit(EXIT_FAILURE);
    }

    if (listen(server_socket, SOMAXCONN) == -1)
    {
        perror("listen\n");
        exit(EXIT_FAILURE);
    }

    int epoll_fd = epoll_create1(0);
    if (epoll_fd == -1)
    {
        perror("epoll_create1\n");
        exit(EXIT_FAILURE);
    }

    struct epoll_event event = add_server_socket_to_epoll(&server_socket, &epoll_fd);

    // Test that the returned event has the correct events and data.fd values
    cr_assert(event.events == EPOLLIN, "event.events should be EPOLLIN");
    cr_assert(event.data.fd == server_socket, "event.data.fd should be server_socket");

    close(server_socket);
    close(epoll_fd);
}

Test(test_accept_client_connection, returns_valid_client_socket)
{
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1)
    {
        perror("socket\n");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in server_address;
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(8082);

    if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) == -1)
    {
        perror("bind\n");
        exit(EXIT_FAILURE);
    }

    if (listen(server_socket, SOMAXCONN) == -1)
    {
        perror("listen\n");
        exit(EXIT_FAILURE);
    }

    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1)
    {
        perror("socket\n");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in client_address;
    memset(&client_address, 0, sizeof(client_address));
    client_address.sin_family = AF_INET;
    client_address.sin_addr.s_addr = inet_addr("127.0.0.1");
    client_address.sin_port = htons(8082);

    if (connect(client_socket, (struct sockaddr *)&client_address, sizeof(client_address)) == -1)
    {
        perror("connect\n");
        exit(EXIT_FAILURE);
    }

    int new_socket = accept_client_connection(&server_socket);

    // Test that the returned socket is valid
    cr_assert(new_socket != -1, "new_socket should not be -1");

    close(new_socket);
    close(client_socket);
    close(server_socket);
}