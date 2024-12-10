
#include "sched.h"
#include "kernel_defines.h"
#include "thread.h"
#include "stdlib.h"
#include "stddef.h"
#include "stdbool.h"
#include "unistd.h"
#include "stdatomic.h"
#include "stdio.h"
#include "event/thread.h"
#include "time_units.h"

#define STACKSIZE 512

static char stack1[STACKSIZE];
static char stack2[STACKSIZE];

void *thread1_func(void *arg)
{
    (void)arg;
    printf("Thread 1 running\n");
    return NULL;
}

void *thread2_func(void *arg)
{
    (void)arg;
    printf("Thread 2 running\n");
    return NULL;
}

int main(void)
{
    kernel_pid_t pid1, pid2;

    // Create Thread 1
    pid1 = thread_create(stack1, sizeof(stack1), THREAD_PRIORITY_MAIN - 1,
                         THREAD_CREATE_STACKTEST, thread1_func, NULL, "thread1");
    if (pid1 <= KERNEL_PID_UNDEF) {
        printf("Failed to create Thread 1\n");
        return 1;
    }

    // Create Thread 2
    pid2 = thread_create(stack2, sizeof(stack2), THREAD_PRIORITY_MAIN - 2,
                         THREAD_CREATE_STACKTEST, thread2_func, NULL, "thread2");
    if (pid2 <= KERNEL_PID_UNDEF) {
        printf("Failed to create Thread 2\n");
        return 1;
    }

    // Change priority of Thread 1
    sched_change_priority(pid1, THREAD_PRIORITY_MAIN - 3);

    // Change priority of Thread 2
    sched_change_priority(pid2, THREAD_PRIORITY_MAIN - 4);

    printf("Main thread running\n");

    return 0;
}
