#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "master/thread_pool.h"
// #include "../crud_operations/http_head.h"
//  #include "../crud_operations/http_post.h"
//  #include "../crud_operations/http_put.h"
//  #include "../crud_operations/http_delete.h"
#include "crud_operations/http_get.h"
#include "crud_operations/http_post.h"
#include "crud_operations/http_delete.h"
#include "crud_operations/http_head.h"
#include "http/http_request/http_request.h"
#include "http/http_response/http_response.h"
#include "http/http_parser/http_parser.h"
#include "http/http_handler/http_handler.h"

#define MAX_THREADS 10
#define MAX_QUEUE_SIZE 100

void *thread_routine(void *arg)
{
    ThreadPool *threadPool = (ThreadPool *)arg;

    while (1)
    {
        pthread_mutex_lock(&(threadPool->queue->mutex));

        // Wait for a task to be available
        while (threadPool->queue->size == 0)
        {
            pthread_cond_wait(&(threadPool->queue->notEmpty), &(threadPool->queue->mutex));
        }

        // Retrieve a task from the queue
        Task *task = threadPool->queue->queue[threadPool->queue->front];
        threadPool->queue->front = (threadPool->queue->front + 1) % threadPool->queue->capacity;
        threadPool->queue->size--;

        // Notify that the queue is not full
        pthread_cond_signal(&(threadPool->queue->notFull));

        pthread_mutex_unlock(&(threadPool->queue->mutex));

        // Process the task
        printf("Processing client request : %d --- in process %d --- in thread : %ld\n", task->clientSocket, getpid(), pthread_self());

        // Create the request object
        struct HttpRequest *http_request = malloc(sizeof(struct HttpRequest));
        if (!parse_http_request(task->clientSocket, http_request))
        {
            printf("Error parsing header request\n");
            continue;
        }

        handle_request(http_request, task->clientSocket);

        free(http_request);
        free(task);
    }

    return NULL;
}

void threadPoolInit(ThreadPool *threadPool, int numThreads)
{
    threadPool->threads = (pthread_t *)malloc(sizeof(pthread_t) * numThreads);
    threadPool->numThreads = numThreads;

    threadPool->queue = (ThreadPoolQueue *)malloc(sizeof(ThreadPoolQueue));
    threadPool->queue->queue = (Task **)malloc(sizeof(Task *) * MAX_QUEUE_SIZE);
    threadPool->queue->front = 0;
    threadPool->queue->rear = -1;
    threadPool->queue->size = 0;
    threadPool->queue->capacity = MAX_QUEUE_SIZE;
    pthread_mutex_init(&(threadPool->queue->mutex), NULL);
    pthread_cond_init(&(threadPool->queue->notEmpty), NULL);
    pthread_cond_init(&(threadPool->queue->notFull), NULL);

    // Create threads
    for (int i = 0; i < numThreads; i++)
    {
        pthread_create(&(threadPool->threads[i]), NULL, thread_routine, (void *)threadPool);
    }
}

void threadPoolEnqueue(ThreadPool *threadPool, Task *task)
{
    pthread_mutex_lock(&(threadPool->queue->mutex));

    // Wait if the queue is full
    while (threadPool->queue->size == threadPool->queue->capacity)
    {
        pthread_cond_wait(&(threadPool->queue->notFull), &(threadPool->queue->mutex));
    }

    // Add the task to the queue
    threadPool->queue->rear = (threadPool->queue->rear + 1) % threadPool->queue->capacity;
    threadPool->queue->queue[threadPool->queue->rear] = task;
    threadPool->queue->size++;

    // Notify that the queue is not empty
    pthread_cond_signal(&(threadPool->queue->notEmpty));

    pthread_mutex_unlock(&(threadPool->queue->mutex));
}
