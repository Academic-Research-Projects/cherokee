/**
 * The above code defines a thread pool implementation in C for handling client requests in a server
 * application.
 */
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

/**
 * The function `thread_routine` is a worker thread function that retrieves tasks from a queue,
 * processes them, and handles client requests.
 * 
 * @param arg The `arg` parameter is a void pointer that is passed to the thread routine function. In
 * this case, it is expected to be a pointer to a `ThreadPool` object. The `ThreadPool` object contains
 * information about the thread pool, including a pointer to the task queue that the threads will be.
 * 
 * @return a `void` pointer, which is `NULL`.
 */
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
    }

    return NULL;
}

/**
 * The function initializes a thread pool with a specified number of threads and sets up the necessary
 * data structures for task management.
 * 
 * @param threadPool A pointer to a ThreadPool struct, which contains information about the thread
 * pool.
 * @param numThreads The `numThreads` parameter specifies the number of threads that will be created in
 * the thread pool.
 */
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

/**
 * The function `threadPoolEnqueue` adds a task to the thread pool's task queue, waiting if the queue
 * is full.
 * 
 * @param threadPool A pointer to the ThreadPool structure, which contains information about the thread
 * pool and its associated queue.
 * @param task A pointer to a Task object that represents the task to be enqueued in the thread pool.
 */
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
