
#include <stdio.h>
#include "thread.h"
#include "sched.h"
#include "xtimer.h"

#define THREAD_STACKSIZE_DEFAULT (THREAD_STACKSIZE_DEFAULT)

char stack1[THREAD_STACKSIZE_DEFAULT];
char stack2[THREAD_STACKSIZE_DEFAULT];

void *thread1_func(void *arg)
{
    (void)arg;
    printf("Thread 1 started with PID: %d\n", thread_getpid());
    xtimer_sleep(5);
    printf("Thread 1 finished\n");
    return NULL;
}

void *thread2_func(void *arg)
{
    (void)arg;
    printf("Thread 2 started with PID: %d\n", thread_getpid());
    xtimer_sleep(5);
    printf("Thread 2 finished\n");
    return NULL;
}

int main(void)
{
    kernel_pid_t pid1, pid2;
    uint8_t priority1, priority2;

    pid1 = thread_create(stack1, sizeof(stack1), THREAD_PRIORITY_MAIN - 1,
                         THREAD_CREATE_STACKTEST, thread1_func, NULL, "thread1");
    pid2 = thread_create(stack2, sizeof(stack2), THREAD_PRIORITY_MAIN - 2,
                         THREAD_CREATE_STACKTEST, thread2_func, NULL, "thread2");

    printf("Main thread: Created thread1 with PID: %d\n", pid1);
    printf("Main thread: Created thread2 with PID: %d\n", pid2);

    xtimer_sleep(2);

    priority1 = thread_get_priority(pid1);
    printf("Main thread: Thread1 priority before change: %d\n", priority1);
    sched_change_priority(pid1, THREAD_PRIORITY_MAIN - 3);
    priority1 = thread_get_priority(pid1);
    printf("Main thread: Thread1 priority after change: %d\n", priority1);

    priority2 = thread_get_priority(pid2);
    printf("Main thread: Thread2 priority before change: %d\n", priority2);
    sched_change_priority(pid2, THREAD_PRIORITY_MAIN - 4);
    priority2 = thread_get_priority(pid2);
    printf("Main thread: Thread2 priority after change: %d\n", priority2);

    return 0;
}
