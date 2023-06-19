#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "worker.h"
#include "thread_pool.h"

#define MAX_THREADS 1
#define MAX_QUEUE_SIZE 10

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

int worker(int *server_socket)
{
    // Create and initialize the thread pool
    ThreadPool threadPool;
    threadPoolInit(&threadPool, MAX_THREADS);

    while (1)
    {
        // Accept a client connection
        // TODO multiplex connections

        int clientSocket = accept(*server_socket, NULL, NULL);

        // Handle the client request
        handleClientRequest(&threadPool, clientSocket);
    }

    // Clean up resources
    // ...

    return 0;
}