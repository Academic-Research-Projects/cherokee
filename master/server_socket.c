#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "master/worker.h"

#define LISTEN_LIMIT 200

// Function to create a socket and enable SO_REUSEPORT option
int create_socket()
{
    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd < 0)
    {
        perror("Error creating socket");
        exit(1);
    }

    int optval = 1;
    if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEPORT, &optval, sizeof(optval)) < 0)
    {
        perror("Error setting SO_REUSEPORT option");
        exit(1);
    }

    return socket_fd;
}

// Function to bind a socket to an IP address and port
void bind_socket(int socket_fd, int port)
{
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port);
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(socket_fd, (struct sockaddr *)&server_address, sizeof(server_address)) < 0)
    {
        perror("Error binding socket");
        exit(1);
    }
}

// Function to listen for incoming connections on a socket
void listen_on_socket(int socket_fd)
{
    if (listen(socket_fd, 10) < 0)
    {
        perror("Error listening");
        exit(1);
    }
}

// Function to create an array of worker sockets and listen for incoming connections
int *create_server_sockets(int port, int num_workers)
{
    int *worker_sockets = (int *)malloc(num_workers * sizeof(int));

    for (int i = 0; i < num_workers; i++)
    {
        worker_sockets[i] = create_socket();
        bind_socket(worker_sockets[i], port);
        listen_on_socket(worker_sockets[i]);
    }

    return worker_sockets;
}

void fork_server(int port)
{
    int num_workers = 3; // Number of worker processes

    int *worker_sockets = create_server_sockets(port, num_workers);

    // Create worker sockets

    pid_t childPid;

    for (int i = 0; i < num_workers; i++)
    {

        childPid = fork();

        if (childPid < 0)
        {
            perror("Error forking the process");
            exit(1);
        }

        if (childPid == 0)
        {
            // This is the child process
            printf("Child process %d\n", getpid());
            worker(&worker_sockets[i]);

            // Close each worker socket
            close(worker_sockets[i]);

            // Exit the child process
            exit(0);
        }
    }
    // Wait for the child processes to exit
    for (int i = 0; i < num_workers; i++)
    {
        wait(NULL);
    }
    free(worker_sockets);
}
