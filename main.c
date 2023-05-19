#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/select.h>
#include <sys/epoll.h>

#define PORT 8080
#define MAX_CLIENTS 16
#define MAX_EVENTS 32

// Function to multiplex connections
void multiplex_connections(int *arg)
{
    int server_socket = *arg;

    // Create epoll instance
    int epoll_fd = epoll_create1(0);
    if (epoll_fd == -1)
    {
        perror("epoll_create1");
        exit(EXIT_FAILURE);
    }

    // Add server socket to epoll instance
    struct epoll_event event;
    event.data.fd = server_socket;
    event.events = EPOLLIN | EPOLLET;
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, server_socket, &event) == -1)
    {
        perror("epoll_ctl: server_socket");
        exit(EXIT_FAILURE);
    }

    // Allocate memory for storing events
    struct epoll_event *events = (struct epoll_event *)malloc(MAX_EVENTS * sizeof(struct epoll_event));
    if (events == NULL)
    {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    while (1)
    {
        int num_events = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
        if (num_events == -1)
        {
            perror("epoll_wait");
            exit(EXIT_FAILURE);
        }
        printf("Number of events: %d\n", num_events);

        for (int i = 0; i < num_events; i++)
        {
            int fd = events[i].data.fd;
            printf("Event on descriptor: %d\n", fd);
        }
    }

    free(events);
    close(epoll_fd);
}

// Function to create a socket
int create_server_socket(int port)
{
    int server_socket;
    struct sockaddr_in server_address;

    // Creating socket file descriptor
    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Set socket to non-blocking
    int flags = fcntl(server_socket, F_GETFL, 0);
    fcntl(server_socket, F_SETFL, flags | O_NONBLOCK);

    // Forcefully attaching socket to the port 8080
    int opt = 1;
    if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)))
    {
        perror("setsockopt failed");
        exit(EXIT_FAILURE);
    }

    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(port);

    // Forcefully attaching socket to the port 8080
    if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) < 0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_socket, 3) < 0)
    {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d\n", port);

    return server_socket;
}

// main() function that starts the server
int main()
{
    int server_socket = create_server_socket(PORT);

    multiplex_connections(&server_socket);

    return 0;
}