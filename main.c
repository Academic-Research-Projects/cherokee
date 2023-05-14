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
#define MAX_EVENTS 10

// thread that handles a client and return http response Hello World
void *cherokee_core(void *arg)
{
    int client_socket = *(int *)arg;

    // read request
    char buffer[1024] = {0};
    read(client_socket, buffer, 1024);
    printf("%s\n", buffer);

    // wait for 10 seconds
    sleep(10);

    // send response
    char *response = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello World!";
    send(client_socket, response, strlen(response), 0);

    return 0;
}

// Main thread that select all client sockets in a select loop and three threads to handle them in parallel
void *multiplex_connection(void *arg)
{
    int server_socket = *(int *)arg;

    // set server socket to non-blocking
    int flags = fcntl(server_socket, F_GETFL, 0);
    fcntl(server_socket, F_SETFL, flags | O_NONBLOCK);

    // initialize epoll instance
    int epfd = epoll_create1(0);
    if (epfd == -1)
    {
        perror("epoll_create1");
        exit(EXIT_FAILURE);
    }

    // add server socket to epoll instance
    struct epoll_event event;
    event.data.fd = server_socket;
    event.events = EPOLLIN | EPOLLET;
    if (epoll_ctl(epfd, EPOLL_CTL_ADD, server_socket, &event) == -1)
    {
        perror("epoll_ctl: server_socket");
        exit(EXIT_FAILURE);
    }

    while (1)
    {
        // wait for events
        struct epoll_event events[MAX_EVENTS];
        int n = epoll_wait(epfd, events, MAX_EVENTS, -1);
        if (n == -1)
        {
            perror("epoll_wait");
            exit(EXIT_FAILURE);
        }

        // loop through events
        for (int i = 0; i < n; i++)
        {
            printf("number of events: %d\n", n);
            // if server socket is ready to read, accept new connection
            if (events[i].data.fd == server_socket)
            {
                // accept new connection
                int client_socket = accept(server_socket, NULL, NULL);
                if (client_socket == -1)
                {
                    perror("accept");
                    exit(EXIT_FAILURE);
                }

                // set client socket to non-blocking
                int flags = fcntl(client_socket, F_GETFL, 0);
                fcntl(client_socket, F_SETFL, flags | O_NONBLOCK);

                // add client socket to epoll instance
                struct epoll_event event;
                event.data.fd = client_socket;
                event.events = EPOLLIN | EPOLLET;
                if (epoll_ctl(epfd, EPOLL_CTL_ADD, client_socket, &event) == -1)
                {
                    perror("epoll_ctl: client_socket");
                    exit(EXIT_FAILURE);
                }
            }
            // if client socket is ready to read, handle request
            else
            {
                // handle request in a new thread
                pthread_t thread;
                pthread_create(&thread, NULL, cherokee_core, &events[i].data.fd);
                pthread_detach(thread);

                // remove client socket from epoll instance when done
                if (epoll_ctl(epfd, EPOLL_CTL_DEL, events[i].data.fd, NULL) == -1)
                {
                    perror("epoll_ctl: client_socket");
                    exit(EXIT_FAILURE);
                }
            }
        }
    }
}

// main() function that starts the server
int main()
{
    int server_socket;
    struct sockaddr_in address;
    int opt = 1;
    // int addrlen = sizeof(address);

    // create server socket
    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // set server socket options
    if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)))
    {
        perror("setsockopt failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // bind server socket to address
    if (bind(server_socket, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // listen for incoming connections
    if (listen(server_socket, MAX_CLIENTS) < 0)
    {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }

    multiplex_connection(&server_socket);

    return 0;
}