
#include <stdio.h>
#include "thread.h"
#include "xtimer.h"

#define THREAD_STACKSIZE_DEFAULT (THREAD_STACKSIZE_MAIN)

kernel_pid_t thread1_pid, thread2_pid;

void *thread1_func(void *arg)
{
    (void)arg;
    printf("Thread 1 started with priority %d\n", thread_get_priority(thread_getpid()));

    // Change priority of Thread 2
    sched_change_priority(thread2_pid, THREAD_PRIORITY_MAIN - 1);
    printf("Changed Thread 2 priority to %d\n", thread_get_priority(thread2_pid));

    return NULL;
}

void *thread2_func(void *arg)
{
    (void)arg;
    printf("Thread 2 started with priority %d\n", thread_get_priority(thread_getpid()));

    // Change priority of Thread 1
    sched_change_priority(thread1_pid, THREAD_PRIORITY_MAIN - 2);
    printf("Changed Thread 1 priority to %d\n", thread_get_priority(thread1_pid));

    return NULL;
}

int main(void)
{
    puts("Starting two threads and changing their priorities at runtime");

    thread1_pid = thread_create(
        thread_stack,
        THREAD_STACKSIZE_DEFAULT,
        THREAD_PRIORITY_MAIN,
        THREAD_CREATE_STACKTEST,
        thread1_func,
        NULL,
        "thread1"
    );

    thread2_pid = thread_create(
        thread_stack,
        THREAD_STACKSIZE_DEFAULT,
        THREAD_PRIORITY_MAIN - 1,
        THREAD_CREATE_STACKTEST,
        thread2_func,
        NULL,
        "thread2"
    );

    return 0;
}