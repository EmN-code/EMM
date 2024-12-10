
#include <stdio.h>
#include "thread.h"
#include "sched.h"

#define THREAD_STACKSIZE_DEFAULT (512)
#define THREAD_PRIORITY_MAIN (THREAD_PRIORITY_MAIN)

char stack1[THREAD_STACKSIZE_DEFAULT];
char stack2[THREAD_STACKSIZE_DEFAULT];

kernel_pid_t pid1, pid2;

void *thread1(void *arg)
{
    (void)arg;
    while (1) {
        printf("Thread 1 running\n");
        thread_yield();
    }
    return NULL;
}

void *thread2(void *arg)
{
    (void)arg;
    while (1) {
        printf("Thread 2 running\n");
        thread_yield();
    }
    return NULL;
}

int main(void)
{
    pid1 = thread_create(stack1, sizeof(stack1), THREAD_PRIORITY_MAIN - 1,
                         THREAD_CREATE_STACKTEST, thread1, NULL, "thread1");
    pid2 = thread_create(stack2, sizeof(stack2), THREAD_PRIORITY_MAIN - 2,
                         THREAD_CREATE_STACKTEST, thread2, NULL, "thread2");

    // Change priority of thread1 to a higher priority
    sched_change_priority(thread_get_thread_pid(pid1), THREAD_PRIORITY_MAIN - 3);

    // Change priority of thread2 to a lower priority
    sched_change_priority(thread_get_thread_pid(pid2), THREAD_PRIORITY_MAIN - 4);

    while (1) {
        thread_yield();
    }

    return 0;
}
