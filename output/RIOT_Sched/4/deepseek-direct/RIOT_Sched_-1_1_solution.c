
#include <stdio.h>
#include "thread.h"
#include "xtimer.h"

#define THREAD_STACKSIZE 512

char thread_stack1[THREAD_STACKSIZE];
char thread_stack2[THREAD_STACKSIZE];

kernel_pid_t thread_pid1, thread_pid2;

void *thread_func1(void *arg)
{
    (void)arg;
    while (1) {
        printf("Thread 1 running at priority %d\n", sched_active_thread->priority);
        xtimer_sleep(2);
    }
    return NULL;
}

void *thread_func2(void *arg)
{
    (void)arg;
    while (1) {
        printf("Thread 2 running at priority %d\n", sched_active_thread->priority);
        xtimer_sleep(2);
    }
    return NULL;
}

int main(void)
{
    puts("Starting two threads with different priorities and changing them at runtime");

    // Create the first thread with priority 5
    thread_pid1 = thread_create(thread_stack1, sizeof(thread_stack1), 5,
                                THREAD_CREATE_STACKTEST, thread_func1, NULL, "thread1");

    // Create the second thread with priority 3
    thread_pid2 = thread_create(thread_stack2, sizeof(thread_stack2), 3,
                                THREAD_CREATE_STACKTEST, thread_func2, NULL, "thread2");

    // Change the priority of the first thread to 2 after 10 seconds
    xtimer_sleep(10);
    thread_change_priority(thread_pid1, 2);
    printf("Changed priority of Thread 1 to 2\n");

    // Change the priority of the second thread to 4 after 10 seconds
    xtimer_sleep(10);
    thread_change_priority(thread_pid2, 4);
    printf("Changed priority of Thread 2 to 4\n");

    // Main thread will also print its priority
    while (1) {
        printf("Main thread running at priority %d\n", sched_active_thread->priority);
        xtimer_sleep(5);
    }

    return 0;
}
