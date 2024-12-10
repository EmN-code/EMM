
#include <stdio.h>
#include "thread.h"
#include "xtimer.h"

#ifndef THREAD_STACKSIZE_DEFAULT
#define THREAD_STACKSIZE_DEFAULT (512)  // Define a default stack size if not already defined
#endif

char thread1_stack[THREAD_STACKSIZE_DEFAULT];
char thread2_stack[THREAD_STACKSIZE_DEFAULT];

void *thread1_func(void *arg)
{
    (void)arg;
    while (1) {
        printf("Thread 1 is running\n");
        xtimer_sleep(1);
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
    kernel_pid_t thread1_pid, thread2_pid;

    // Create the first thread
    thread1_pid = thread_create(thread1_stack, sizeof(thread1_stack),
                                THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST,
                                thread1_func, NULL, "thread1");

    // Create the second thread
    thread2_pid = thread_create(thread2_stack, sizeof(thread2_stack),
                                THREAD_PRIORITY_MAIN - 2, THREAD_CREATE_STACKTEST,
                                thread2_func, NULL, "thread2");

    // Change the priority of the first thread at runtime
    sched_change_priority(thread_get(thread1_pid), THREAD_PRIORITY_MAIN - 3);

    // Change the priority of the second thread at runtime
    sched_change_priority(thread_get(thread2_pid), THREAD_PRIORITY_MAIN - 4);

    while (1) {
        printf("Main thread is running\n");
        xtimer_sleep(3);
    }

    return 0;
}
