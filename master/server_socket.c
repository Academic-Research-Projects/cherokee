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

#include "worker.h"

#define LISTEN_LIMIT 200

// Function to create a socket
int *create_server_sockets(int port, int num_workers)
{
    // Create an array of worker sockets
    int *worker_sockets = (int *)malloc(num_workers * sizeof(int));

    for (int i = 0; i < num_workers; i++)
    {
        worker_sockets[i] = socket(AF_INET, SOCK_STREAM, 0);
        if (worker_sockets[i] < 0)
        {
            perror("Error creating socket");
            exit(1);
        }

        // Enable SO_REUSEPORT option
        int optval = 1;
        if (setsockopt(worker_sockets[i], SOL_SOCKET, SO_REUSEPORT, &optval, sizeof(optval)) < 0)
        {
            perror("Error setting SO_REUSEPORT option");
            exit(1);
        }

        // Bind socket to IP address and port
        struct sockaddr_in server_address;
        server_address.sin_family = AF_INET;
        server_address.sin_port = htons(port); // Port number
        server_address.sin_addr.s_addr = htonl(INADDR_ANY);

        if (bind(worker_sockets[i], (struct sockaddr *)&server_address, sizeof(server_address)) < 0)
        {
            perror("Error binding socket");
            exit(1);
        }

        // Listen for incoming connections
        if (listen(worker_sockets[i], 10) < 0)
        {
            perror("Error listening");
            exit(1);
        }
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
    for (int i = 0; i < 3; i++)
    {
        wait(NULL);
    }
}
