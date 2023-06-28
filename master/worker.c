#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/select.h>
#include <sys/epoll.h>
#include <signal.h>
#include <arpa/inet.h>
#include "worker.h"
#include "thread_pool.h"
#include "multiplex.h"
#include <stdbool.h>

#define MAX_THREADS 10
#define MAX_QUEUE_SIZE 10
#define MAX_EVENTS 1000

int epoll_fd;
struct epoll_event *events;
bool loop = true;

// Signal handler for SIGINT
void handle_sigint(int sig)
{
    printf("Received signal %d, shutting down...\n", sig);

    close(epoll_fd);
    free(events);

    loop = false;
}

void handleClientRequest(ThreadPool *threadPool, int clientSocket)
{
    // TODO Process the client request
    // ...

    // Create a task
    Task *task = (Task *)malloc(sizeof(Task));
    task->clientSocket = clientSocket;
    // TODO Set the task arguments

    // Enqueue the task to the thread pool
    threadPoolEnqueue(threadPool, task);
}

int worker(int *arg)
{
    // Create and initialize the thread pool
    ThreadPool threadPool;
    threadPoolInit(&threadPool, MAX_THREADS);

    struct sockaddr_in client_addr;
    int server_socket = *arg;

    // Create epoll instance
    epoll_fd = epoll_create1(0);
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
    events = malloc(MAX_EVENTS * sizeof(struct epoll_event));
    if (events == NULL)
    {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    while (loop)
    {
        // Add signal handler for SIGINT
        signal(SIGINT, handle_sigint);

        int num_events = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);

        for (int i = 0; i < num_events; i++)
        {
            if (events[i].data.fd == server_socket)
            {
                // new client connection
                int new_socket, addrlen = sizeof(client_addr);
                if ((new_socket = accept(server_socket, (struct sockaddr *)&client_addr, (socklen_t *)&addrlen)) < 0)
                {
                    // if cant accept ignore and continue
                    perror("accept");
                    continue;
                }

                // printf("New client connection, process is %d, socket fd is %d, IP is %s, port is %d\n",
                //        getpid(), new_socket, inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

                event.events = EPOLLIN | EPOLLET;
                event.data.fd = new_socket;

                // Add new client socket to epoll instance
                if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, new_socket, &event) == -1)
                {
                    perror("epoll_ctl: new_socket");
                    exit(EXIT_FAILURE);
                }
            }
            else
            {
                // existing client connection
                int client_socket = events[i].data.fd;
                handleClientRequest(&threadPool, client_socket);

                // Reinitialize event structure
                event.data.fd = -1;
                event.events = 0;
                // }
            }
        }
    }
    return 0;
}