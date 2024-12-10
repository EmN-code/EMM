
#include <stdio.h>
#include "thread.h"
#include "xtimer.h"

#define THREAD_STACKSIZE_DEFAULT (THREAD_STACKSIZE_MAIN)

kernel_pid_t thread1_pid, thread2_pid;

void *thread1_func(void *arg)
{
    (void)arg;
    while (1) {
        printf("Thread 1 is running\n");
        xtimer_sleep(2);
    }
    return NULL;
}

void *thread2_func(void *arg)
{
    (void)arg;
    while (1) {
        printf("Thread 2 is running\n");
        xtimer_sleep(2);
    }
    return NULL;
}

int main(void)
{
    puts("Starting two threads...");

    // Create and start the first thread
    thread1_pid = thread_create(
        thread_stack,
        THREAD_STACKSIZE_DEFAULT,
        THREAD_PRIORITY_MAIN - 1,
        THREAD_CREATE_STACKTEST,
        thread1_func,
        NULL,
        "thread1"
    );

    // Create and start the second thread
    thread2_pid = thread_create(
        thread_stack,
        THREAD_STACKSIZE_DEFAULT,
        THREAD_PRIORITY_MAIN - 2,
        THREAD_CREATE_STACKTEST,
        thread2_func,
        NULL,
        "thread2"
    );

    // Change the priority of thread1 to a higher priority
    thread_change_priority(thread1_pid, THREAD_PRIORITY_MAIN - 3);

    // Change the priority of thread2 to a lower priority
    thread_change_priority(thread2_pid, THREAD_PRIORITY_MAIN - 4);

    puts("Thread priorities changed at runtime.");

    return 0;
}
