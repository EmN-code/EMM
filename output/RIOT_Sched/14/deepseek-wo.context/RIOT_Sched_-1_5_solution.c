
#include <stdio.h>
#include "thread.h"
#include "msg.h"
#include "xtimer.h"

#define THREAD_STACKSIZE_DEFAULT (512)
#define DEBUG_EXTRA_STACKSIZE (0)

kernel_pid_t thread1_pid, thread2_pid;

void *thread1_func(void *arg)
{
    (void)arg;
    printf("Thread 1 started with PID %d\n", thread_getpid());
    while (1) {
        xtimer_sleep(1);
    }
    return NULL;
}

void *thread2_func(void *arg)
{
    (void)arg;
    printf("Thread 2 started with PID %d\n", thread_getpid());
    while (1) {
        xtimer_sleep(1);
    }
    return NULL;
}

int main(void)
{
    char stack1[THREAD_STACKSIZE_DEFAULT + DEBUG_EXTRA_STACKSIZE];
    char stack2[THREAD_STACKSIZE_DEFAULT + DEBUG_EXTRA_STACKSIZE];

    thread1_pid = thread_create(stack1, sizeof(stack1), THREAD_PRIORITY_MAIN - 1,
                                THREAD_CREATE_STACKTEST, thread1_func, NULL, "thread1");

    thread2_pid = thread_create(stack2, sizeof(stack2), THREAD_PRIORITY_MAIN - 2,
                                THREAD_CREATE_STACKTEST, thread2_func, NULL, "thread2");

    xtimer_sleep(5);

    printf("Changing priority of Thread 1 to higher\n");
    sched_change_priority(thread_get_active(), THREAD_PRIORITY_MAIN - 2);

    xtimer_sleep(5);

    printf("Changing priority of Thread 2 to higher\n");
    sched_change_priority(thread_get_active(), THREAD_PRIORITY_MAIN - 1);

    return 0;
}
