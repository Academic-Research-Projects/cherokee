#ifndef WORKER_H
#define WORKER_H

#include <pthread.h>

typedef struct
{
    int clientSocket;
    // Add any additional data you need to pass to the worker function
    // ...
} Task;

typedef struct
{
    Task **queue;
    int front;
    int rear;
    int size;
    int capacity;
    pthread_mutex_t mutex;
    pthread_cond_t notEmpty;
    pthread_cond_t notFull;
} ThreadPoolQueue;

typedef struct
{
    pthread_t *threads;
    int numThreads;
    ThreadPoolQueue *queue;
    // Add any additional data you need for the thread pool
    // ...
} ThreadPool;

int worker(int *server_socket);

#endif