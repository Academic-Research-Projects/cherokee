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
#define MAX_EVENTS 1000
#define LISTEN_LIMIT 200

// Function to multiplex connections
void multiplex_connections(int *arg)
{
    struct sockaddr_in client_addr;
    char buffer[1024] = {0};
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
    event.events = EPOLLIN;
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
        printf("Number of events: %d\n", num_events);

        for (int i = 0; i < num_events; i++)
        {
            if (events[i].data.fd == server_socket)
            {
                // new client connection
                int new_socket, addrlen = sizeof(client_addr);
                if ((new_socket = accept(server_socket, (struct sockaddr *)&client_addr, (socklen_t *)&addrlen)) < 0)
                {
                    perror("accept");
                    exit(EXIT_FAILURE);
                };

                printf("New client connection, socket fd is %d, IP is %s, port is %d\n",
                       new_socket, inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

                event.events = EPOLLIN | EPOLLET;
                event.data.fd = new_socket;

                // Add new client socket to epoll instance
                if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, new_socket, &event) == -1)
                {
                    perror("epoll_ctl: new_socket");
                    exit(EXIT_FAILURE);
                }
                printf("Added new client socket to epoll instance\n");
            }
            else
            {
                // existing client connection
                int client_socket = events[i].data.fd;
                int valread = read(client_socket, buffer, 1024);
                if (valread == 0)
                {
                    // client disconnected
                    printf("Client disconnected, socket fd is %d\n", client_socket);
                    close(client_socket);
                }
                else
                {
                    // client sent a message
                    printf("Client sent a message, socket fd is %d\n", client_socket);
                    // send message back to client in HTTP format
                    char *message = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!";
                    send(client_socket, message, strlen(message), 0);

                    // Remove client socket from epoll instance
                    if (epoll_ctl(epoll_fd, EPOLL_CTL_DEL, client_socket, NULL) == -1)
                    {
                        perror("epoll_ctl: client_socket");
                        exit(EXIT_FAILURE);
                    }

                    // close client connection
                    close(client_socket);
                    printf("Closed client socket\n-----------------\n");
                }
            }
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

    if (listen(server_socket, LISTEN_LIMIT) < 0)
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