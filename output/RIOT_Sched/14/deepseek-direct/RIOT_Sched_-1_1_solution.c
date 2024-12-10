
#include <stdio.h>
#include "thread.h"
#include "xtimer.h"

#define THREAD_STACKSIZE 512

char stack1[THREAD_STACKSIZE];
char stack2[THREAD_STACKSIZE];

kernel_pid_t pid1, pid2;

void *thread1(void *arg)
{
    (void)arg;
    while (1) {
        printf("Thread 1 running at priority %d\n", sched_active_thread->priority);
        xtimer_sleep(2);
    }
    return NULL;
}

void *thread2(void *arg)
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
    puts("Starting two threads with different priorities...");

    // Create thread 1 with priority 5
    pid1 = thread_create(stack1, sizeof(stack1), 5, THREAD_CREATE_STACKTEST, thread1, NULL, "thread1");

    // Create thread 2 with priority 3
    pid2 = thread_create(stack2, sizeof(stack2), 3, THREAD_CREATE_STACKTEST, thread2, NULL, "thread2");

    // Change priority of thread 1 to 2 after 10 seconds
    xtimer_sleep(10);
    thread_change_priority(pid1, 2);
    printf("Changed priority of thread 1 to 2\n");

    // Change priority of thread 2 to 4 after 20 seconds
    xtimer_sleep(10);
    thread_change_priority(pid2, 4);
    printf("Changed priority of thread 2 to 4\n");

    return 0;
}
