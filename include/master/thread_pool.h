#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <stdbool.h>

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
    bool terminate_flag;
    // Add any additional data you need for the thread pool
    // ...
} ThreadPool;

void add_task_to_queue(ThreadPoolQueue *queue, Task *task);

Task *get_task_from_queue(ThreadPoolQueue *queue);

void *thread_routine(void *arg);

pthread_t *threadPoolInit(ThreadPool *threadPool, int numThreads);

void threadPoolEnqueue(ThreadPool *threadPool, Task *task);

void handleClientRequest(ThreadPool *threadPool, int clientSocket);

#endif