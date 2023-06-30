#ifndef THREAD_POOL_H
#define THREAD_POOL_H

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

void *thread_routine(void *arg);

void threadPoolInit(ThreadPool *threadPool, int numThreads);

void threadPoolEnqueue(ThreadPool *threadPool, Task *task);

void handleClientRequest(ThreadPool *threadPool, int clientSocket);

#endif