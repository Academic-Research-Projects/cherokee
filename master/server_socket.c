/**
 * The above code is a C program that creates multiple worker processes to handle incoming connections
 * on different sockets.
 *
 * @return The function `create_server_sockets` returns a pointer to an array of integers, which
 * represents the worker sockets.
 */
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
/**
 * The function creates a socket and sets the SO_REUSEPORT option.
 *
 * @return the file descriptor of the created socket.
 */
int create_socket()
{
    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd < 0)
    {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }

    int optval = 1;
    if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEPORT, &optval, sizeof(optval)) < 0)
    {
        perror("Error setting SO_REUSEPORT option");
        exit(EXIT_FAILURE);
    }

    return socket_fd;
}

// Function to bind a socket to an IP address and port
/**
 * The function `bind_socket` binds a socket to a specific port.
 *
 * @param socket_fd The socket file descriptor, which is a unique identifier for the socket that is
 * being bound.
 * @param port The "port" parameter is an integer that represents the port number on which the socket
 * will be bound. The port number is used to identify a specific process or service on a network. It is
 * a 16-bit unsigned integer ranging from 0 to 65535.
 */
void bind_socket(int socket_fd, int port)
{
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port);
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(socket_fd, (struct sockaddr *)&server_address, sizeof(server_address)) < 0)
    {
        perror("Error binding socket");
        exit(EXIT_FAILURE);
    }
}

// Function to listen for incoming connections on a socket
/**
 * The function listens for incoming connections on a specified socket.
 *
 * @param socket_fd The parameter `socket_fd` is the file descriptor of the socket that you want to
 * listen on.
 */
void listen_on_socket(int socket_fd)
{
    if (listen(socket_fd, 10) < 0)
    {
        perror("Error listening");
        exit(EXIT_FAILURE);
    }
}

// Function to create an array of worker sockets and listen for incoming connections
/**
 * The function creates and initializes a specified number of server sockets for a given port.
 *
 * @param port The `port` parameter is an integer that represents the port number on which the server
 * sockets will be created and bound. It specifies the network port that the server will listen on for
 * incoming connections.
 * @param num_workers The parameter "num_workers" represents the number of worker sockets that need to
 * be created and set up for the server.
 *
 * @return The function `create_server_sockets` returns a pointer to an array of integers, which
 * represents the worker sockets created for the server.
 */
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

/**
 * The function `fork_server` creates multiple worker processes to handle incoming connections on a
 * specified port.
 *
 * @param port The `port` parameter is the port number on which the server will listen for incoming
 * connections.
 */
void fork_server(int port)
{
    int num_workers = 3; // Number of worker processes

    pid_t childPids[num_workers];
    int worker_sockets[num_workers];

    for (int i = 0; i < num_workers; i++)
    {
        childPids[i] = fork();

        if (childPids[i] < 0)
        {
            perror("Error forking the process");
            exit(EXIT_FAILURE);
        }

        if (childPids[i] == 0)
        {
            // This is the child process
            printf("Child process %d\n", getpid());

            // Create the worker socket
            worker_sockets[i] = create_socket();
            bind_socket(worker_sockets[i], port);
            listen_on_socket(worker_sockets[i]);

            worker(&worker_sockets[i]);

            // Close the worker socket
            close(worker_sockets[i]);

            // Exit the child process
            // exit(EXIT_SUCCESS);
        }
    }

    // Wait for the child processes to exit
    for (int i = 0; i < num_workers; i++)
    {
        waitpid(childPids[i], NULL, 0);
    }
    return;
}
