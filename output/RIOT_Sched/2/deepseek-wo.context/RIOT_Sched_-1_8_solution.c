
#include <stdio.h>
#include "thread.h"
#include "sched.h"
#include "xtimer.h"

#define THREAD_STACKSIZE_DEFAULT (THREAD_STACKSIZE_DEFAULT)
#define DEBUG_EXTRA_STACKSIZE (0)

kernel_pid_t thread1_pid, thread2_pid;

void *thread1_func(void *arg)
{
    (void)arg;
    while (1) {
        printf("Thread 1 running\n");
        xtimer_sleep(1);
    }
    return NULL;
}

void *thread2_func(void *arg)
{
    (void)arg;
    while (1) {
        printf("Thread 2 running\n");
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

    thread_t *thread1 = thread_get_from_pid(thread1_pid);
    thread_t *thread2 = thread_get_from_pid(thread2_pid);

    sched_change_priority(thread1, THREAD_PRIORITY_MAIN - 2);
    sched_change_priority(thread2, THREAD_PRIORITY_MAIN - 1);

    printf("Thread priorities changed\n");

    return 0;
}
