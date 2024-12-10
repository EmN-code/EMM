
#include <stdio.h>
#include "thread.h"
#include "xtimer.h"

#ifndef THREAD_STACKSIZE_DEFAULT
#define THREAD_STACKSIZE_DEFAULT (512)  // Define a default stack size if not already defined
#endif

char thread1_stack[THREAD_STACKSIZE_DEFAULT];
char thread2_stack[THREAD_STACKSIZE_DEFAULT];

kernel_pid_t thread1_pid, thread2_pid;

void *thread1_func(void *arg)
{
    (void)arg;
    printf("Thread 1 started with priority %d\n", thread_get_priority(thread_getpid()));
    xtimer_sleep(5);
    printf("Thread 1 changing priority to higher\n");
    sched_change_priority(thread_getpid(), THREAD_PRIORITY_MAIN - 1);
    printf("Thread 1 new priority: %d\n", thread_get_priority(thread_getpid()));
    return NULL;
}

void *thread2_func(void *arg)
{
    (void)arg;
    printf("Thread 2 started with priority %d\n", thread_get_priority(thread_getpid()));
    xtimer_sleep(5);
    printf("Thread 2 changing priority to lower\n");
    sched_change_priority(thread_getpid(), THREAD_PRIORITY_MAIN + 1);
    printf("Thread 2 new priority: %d\n", thread_get_priority(thread_getpid()));
    return NULL;
}

int main(void)
{
    printf("Starting two threads...\n");

    thread1_pid = thread_create(thread1_stack, sizeof(thread1_stack),
                                THREAD_PRIORITY_MAIN, 0,
                                thread1_func, NULL, "thread1");

    thread2_pid = thread_create(thread2_stack, sizeof(thread2_stack),
                                THREAD_PRIORITY_MAIN, 0,
                                thread2_func, NULL, "thread2");

    printf("Threads started. Main thread exiting.\n");
    return 0;
}
