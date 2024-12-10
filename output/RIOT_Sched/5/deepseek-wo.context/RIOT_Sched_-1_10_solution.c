
#include <stdio.h>
#include "thread.h"
#include "sched.h"
#include "kernel_defines.h"

#define THREAD_STACKSIZE_DEFAULT (512)

char stack1[THREAD_STACKSIZE_DEFAULT];
char stack2[THREAD_STACKSIZE_DEFAULT];

kernel_pid_t pid1, pid2;

void *thread1(void *arg)
{
    (void)arg;
    printf("Thread 1 running\n");
    return NULL;
}

void *thread2(void *arg)
{
    (void)arg;
    printf("Thread 2 running\n");
    return NULL;
}

int main(void)
{
    pid1 = thread_create(stack1, sizeof(stack1), THREAD_PRIORITY_MAIN - 1,
                         THREAD_CREATE_STACKTEST, thread1, NULL, "thread1");

    pid2 = thread_create(stack2, sizeof(stack2), THREAD_PRIORITY_MAIN - 2,
                         THREAD_CREATE_STACKTEST, thread2, NULL, "thread2");

    // Change priority of thread1 to higher priority
    sched_change_priority(pid1, THREAD_PRIORITY_MAIN - 3);

    // Change priority of thread2 to lower priority
    sched_change_priority(pid2, THREAD_PRIORITY_MAIN - 1);

    return 0;
}
