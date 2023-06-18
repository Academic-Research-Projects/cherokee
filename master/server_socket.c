#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>

#include "master/worker.h"

#define LISTEN_LIMIT 200

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

    // Bind the socket to the server address
    if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) < 0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // Start listening for client connections
    if (listen(server_socket, LISTEN_LIMIT) < 0)
    {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }

    // printf("Server listening on port %d\n", port);

    return server_socket;
}

void fork_server(int *server_socket)
{
    int clientSocket;
    struct sockaddr_in serverAddress, clientAddress;
    socklen_t clientLength;
    pid_t childPid;

    for (int i = 0; i < 3; i++)
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
            worker(server_socket);

            // Exit the child process
            exit(0);
        }
    }

    // Close the server socket in the parent process
    close(server_socket);

    // Wait for the child processes to exit
    for (int i = 0; i < 3; i++)
    {
        wait(NULL);
    }

    return 0;
}
