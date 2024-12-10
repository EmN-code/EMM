
#include "thread.h"
#include "sched.h"
#include "mutex.h"
#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
#include "stdbool.h"
#include "stddef.h"
#include "stdatomic.h"

#define THREAD_STACKSIZE_DEFAULT 512

static char stack1[THREAD_STACKSIZE_DEFAULT];
static char stack2[THREAD_STACKSIZE_DEFAULT];

static void *thread_func1(void *arg)
{
    (void)arg;
    while (1) {
        printf("Thread 1 running\n");
        thread_yield();
    }
    return NULL;
}

static void *thread_func2(void *arg)
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
    kernel_pid_t pid1, pid2;

    pid1 = thread_create(stack1, sizeof(stack1), THREAD_PRIORITY_MAIN - 1,
                         THREAD_CREATE_WOUT_YIELD, thread_func1, NULL, "thread1");
    pid2 = thread_create(stack2, sizeof(stack2), THREAD_PRIORITY_MAIN - 2,
                         THREAD_CREATE_WOUT_YIELD, thread_func2, NULL, "thread2");

    printf("Threads created with PIDs: %d and %d\n", pid1, pid2);

    // Change priority of thread 1 to higher
    sched_change_priority(thread_get(pid1), THREAD_PRIORITY_MAIN - 3);
    printf("Changed priority of thread 1 to higher\n");

    // Change priority of thread 2 to lower
    sched_change_priority(thread_get(pid2), THREAD_PRIORITY_MAIN - 1);
    printf("Changed priority of thread 2 to lower\n");

    while (1) {
        thread_yield();
    }

    return 0;
}
