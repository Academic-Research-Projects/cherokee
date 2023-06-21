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
#include "crud_operations/http_get.h"
#include "http/http_request/http_request.h"
#include "http/http_response/http_response.h"
#include "http/http_parser/http_parser.h"

#define MAX_EVENTS 1000

// Function to multiplex connections
void multiplex_connections(int *arg)
{
    struct sockaddr_in client_addr;
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
    struct epoll_event *events = malloc(MAX_EVENTS * sizeof(struct epoll_event));
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

                struct HttpRequest *http_request;
                if (!parse_http_request(client_socket, &http_request))
                {
                    printf("Error parsing header request\n");
                    continue;
                }
                printf("Method: %s\n", http_request.request_line.method);

                // handle_request(&http_request, client_socket);
                // http_get(&client_socket);

                // Reinitialize event structure
                event.data.fd = -1;
                event.events = 0;
                // }
            }
        }
    }

    free(events);
    close(epoll_fd);
}