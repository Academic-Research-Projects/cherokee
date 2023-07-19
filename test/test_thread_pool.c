#include <criterion/criterion.h>
#include <pthread.h>

#include "master/thread_pool.h"

#define MAX_THREADS 10
#define MAX_QUEUE_SIZE 100

ThreadPool thread_pool;

Test(thread_pool, threadPoolInit) {
    
    thread_pool.threads = threadPoolInit(&thread_pool, MAX_THREADS);

    cr_assert_not_null(thread_pool.threads, "Failed to create threads");
    cr_assert_eq(thread_pool.numThreads, MAX_THREADS, "Unexpected number of threads : %d", thread_pool.numThreads);
    cr_assert_eq(thread_pool.terminate_flag, false, "Unexpected terminate flag : %d", thread_pool.terminate_flag);
    cr_assert_not_null(thread_pool.queue, "Failed to create queue");
    cr_assert_eq(thread_pool.queue->capacity, MAX_QUEUE_SIZE, "Unexpected queue capacity : %d", thread_pool.queue->capacity);
    cr_assert_eq(thread_pool.queue->size, 0, "Unexpected queue size : %d", thread_pool.queue->size);
}

Test(thread_pool, add_task_to_queue) {
    ThreadPoolQueue *queue = malloc(sizeof(ThreadPoolQueue));
    queue->queue = malloc(sizeof(Task *) * MAX_QUEUE_SIZE);
    queue->front = 0;
    queue->rear = -1;
    queue->size = 0;
    queue->capacity = MAX_QUEUE_SIZE;

    Task task1;
    task1.clientSocket = 1;
    Task task2;
    task2.clientSocket = 2;
    add_task_to_queue(queue, &task1);
    add_task_to_queue(queue, &task2);

    // Check that the queue size is 2
    cr_assert_eq(queue->size, 2, "Unexpected queue size %d", queue->size);

    // Check that the first task added to the queue is at the front of the queue
    cr_assert_eq(queue->queue[queue->front], &task1, "Unexpected task at front of queue");

    // Check that the second task added to the queue is at the rear of the queue
    cr_assert_eq(queue->queue[queue->rear], &task2, "Unexpected task at rear of queue");

    free(queue->queue);
    free(queue);
}

Test(thread_pool, get_task_from_queue) {
    ThreadPoolQueue *queue = malloc(sizeof(ThreadPoolQueue));
    queue->queue = malloc(sizeof(Task *) * MAX_QUEUE_SIZE);
    queue->front = 0;
    queue->rear = -1;
    queue->size = 0;
    queue->capacity = MAX_QUEUE_SIZE;

    Task task1;
    task1.clientSocket = 1;
    Task task2;
    task2.clientSocket = 2;
    add_task_to_queue(queue, &task1);
    add_task_to_queue(queue, &task2);

    Task *task = get_task_from_queue(queue);

    // Check that the task returned is the first task added to the queue
    cr_assert_eq(task, &task1, "Unexpected task returned from queue");

    // Check that the queue size is 1
    cr_assert_eq(queue->size, 1, "Unexpected queue size %d", queue->size);

    // Check that the task at the front of the queue is the second task added to the queue
    cr_assert_eq(queue->queue[queue->front], &task2, "Unexpected task at front of queue");

    free(queue->queue);
    free(queue);
}