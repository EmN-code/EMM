
#include <stdio.h>
#include <inttypes.h>
#include "event/thread.h"
#include "thread.h"
#include "sched.h"

#define STACKSIZE 512

char stack1[STACKSIZE];
char stack2[STACKSIZE];

kernel_pid_t pid1, pid2;

void *thread1_func(void *arg)
{
    (void)arg;
    printf("Thread 1 started\n");
    while (1) {
        printf("Thread 1 running\n");
        thread_yield();
    }
    return NULL;
}

void *thread2_func(void *arg)
{
    (void)arg;
    printf("Thread 2 started\n");
    while (1) {
        printf("Thread 2 running\n");
        thread_yield();
    }
    return NULL;
}

int main(void)
{
    puts("RIOT application starting");

    // Create the first thread
    pid1 = thread_create(stack1, sizeof(stack1), THREAD_PRIORITY_MAIN - 1,
                         THREAD_CREATE_STACKTEST, thread1_func, NULL, "thread1");

    // Create the second thread
    pid2 = thread_create(stack2, sizeof(stack2), THREAD_PRIORITY_MAIN - 2,
                         THREAD_CREATE_STACKTEST, thread2_func, NULL, "thread2");

    // Change the priority of the first thread at runtime
    sched_change_priority(thread_get(pid1), THREAD_PRIORITY_MAIN - 3);

    // Change the priority of the second thread at runtime
    sched_change_priority(thread_get(pid2), THREAD_PRIORITY_MAIN - 4);

    // Main thread continues to run
    while (1) {
        printf("Main thread running\n");
        thread_yield();
    }

    return 0;
}
