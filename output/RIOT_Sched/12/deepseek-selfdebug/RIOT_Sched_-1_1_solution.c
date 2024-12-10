
#include <stdio.h>
#include "thread.h"
#include "xtimer.h"

#define THREAD_STACKSIZE_DEFAULT (THREAD_STACKSIZE_MAIN)

kernel_pid_t thread1_pid, thread2_pid;
char thread1_stack[THREAD_STACKSIZE_DEFAULT];
char thread2_stack[THREAD_STACKSIZE_DEFAULT];

void *thread1_func(void *arg)
{
    (void)arg;
    printf("Thread 1 started with priority %d\n", thread_get_priority(thread_getpid()));

    while (1) {
        printf("Thread 1 running\n");
        xtimer_sleep(1);
    }

    return NULL;
}

void *thread2_func(void *arg)
{
    (void)arg;
    printf("Thread 2 started with priority %d\n", thread_get_priority(thread_getpid()));

    while (1) {
        printf("Thread 2 running\n");
        xtimer_sleep(1);
    }

    return NULL;
}

int main(void)
{
    // Create and start the first thread with default priority
    thread1_pid = thread_create(thread1_stack, sizeof(thread1_stack),
                                THREAD_PRIORITY_MAIN, THREAD_CREATE_STACKTEST,
                                thread1_func, NULL, "thread1");

    // Create and start the second thread with default priority
    thread2_pid = thread_create(thread2_stack, sizeof(thread2_stack),
                                THREAD_PRIORITY_MAIN, THREAD_CREATE_STACKTEST,
                                thread2_func, NULL, "thread2");

    // Change the priority of thread1 to a higher priority
    thread_change_priority(thread1_pid, THREAD_PRIORITY_MAIN - 1);
    printf("Changed Thread 1 priority to %d\n", thread_get_priority(thread1_pid));

    // Change the priority of thread2 to a lower priority
    thread_change_priority(thread2_pid, THREAD_PRIORITY_MAIN + 1);
    printf("Changed Thread 2 priority to %d\n", thread_get_priority(thread2_pid));

    // Main thread will also run and print its priority
    printf("Main thread running with priority %d\n", thread_get_priority(thread_getpid()));

    while (1) {
        printf("Main thread running\n");
        xtimer_sleep(2);
    }

    return 0;
}
