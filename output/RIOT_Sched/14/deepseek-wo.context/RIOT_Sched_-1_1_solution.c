
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
    while (1) {
        printf("Thread 1 running at priority %d\n", thread_get_priority(thread_getpid()));
        xtimer_sleep(2);
    }
    return NULL;
}

void *thread2_func(void *arg)
{
    (void)arg;
    while (1) {
        printf("Thread 2 running at priority %d\n", thread_get_priority(thread_getpid()));
        xtimer_sleep(2);
    }
    return NULL;
}

int main(void)
{
    // Start thread 1 with default priority
    thread1_pid = thread_create(thread1_stack, sizeof(thread1_stack),
                                THREAD_PRIORITY_MAIN, THREAD_CREATE_STACKTEST,
                                thread1_func, NULL, "thread1");

    // Start thread 2 with default priority
    thread2_pid = thread_create(thread2_stack, sizeof(thread2_stack),
                                THREAD_PRIORITY_MAIN, THREAD_CREATE_STACKTEST,
                                thread2_func, NULL, "thread2");

    // Change priority of thread 1 to higher
    thread_change_priority(thread1_pid, THREAD_PRIORITY_MAIN - 1);

    // Change priority of thread 2 to lower
    thread_change_priority(thread2_pid, THREAD_PRIORITY_MAIN + 1);

    // Main thread will also run and print its priority
    while (1) {
        printf("Main thread running at priority %d\n", thread_get_priority(thread_getpid()));
        xtimer_sleep(2);
    }

    return 0;
}
