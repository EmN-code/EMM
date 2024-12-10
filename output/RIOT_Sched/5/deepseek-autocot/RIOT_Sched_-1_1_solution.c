
#include <stdio.h>
#include "thread.h"
#include "xtimer.h"

#define THREAD_STACKSIZE_DEFAULT (THREAD_STACKSIZE_MAIN)

kernel_pid_t thread1_pid, thread2_pid;

void *thread1_func(void *arg)
{
    (void)arg;
    printf("Thread 1 started with priority %d\n", thread_get_priority(thread_getpid()));

    while (1) {
        printf("Thread 1 running\n");
        xtimer_sleep(2);
    }

    return NULL;
}

void *thread2_func(void *arg)
{
    (void)arg;
    printf("Thread 2 started with priority %d\n", thread_get_priority(thread_getpid()));

    while (1) {
        printf("Thread 2 running\n");
        xtimer_sleep(2);
    }

    return NULL;
}

int main(void)
{
    puts("Starting two threads and changing their priorities at runtime");

    // Create and start thread 1 with default priority
    thread1_pid = thread_create(
        thread_stack,
        THREAD_STACKSIZE_DEFAULT,
        THREAD_PRIORITY_MAIN,
        THREAD_CREATE_STACKTEST,
        thread1_func,
        NULL,
        "thread1"
    );

    // Create and start thread 2 with default priority
    thread2_pid = thread_create(
        thread_stack,
        THREAD_STACKSIZE_DEFAULT,
        THREAD_PRIORITY_MAIN,
        THREAD_CREATE_STACKTEST,
        thread2_func,
        NULL,
        "thread2"
    );

    // Change the priority of thread 1 to higher
    thread_change_priority(thread1_pid, THREAD_PRIORITY_MAIN - 1);
    printf("Changed priority of Thread 1 to %d\n", thread_get_priority(thread1_pid));

    // Change the priority of thread 2 to lower
    thread_change_priority(thread2_pid, THREAD_PRIORITY_MAIN + 1);
    printf("Changed priority of Thread 2 to %d\n", thread_get_priority(thread2_pid));

    // Main thread will also run and print messages
    while (1) {
        printf("Main thread running\n");
        xtimer_sleep(5);
    }

    return 0;
}
