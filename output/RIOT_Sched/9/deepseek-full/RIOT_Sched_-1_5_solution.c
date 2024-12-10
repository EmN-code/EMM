
#include "thread.h"
#include "sched.h"
#include "xtimer.h"
#include "stdio.h"

#define STACKSIZE 512

kernel_pid_t thread1_pid, thread2_pid;

void *thread1_func(void *arg)
{
    (void)arg;
    printf("Thread 1 running\n");
    while (1) {
        xtimer_sleep(1);
    }
    return NULL;
}

void *thread2_func(void *arg)
{
    (void)arg;
    printf("Thread 2 running\n");
    while (1) {
        xtimer_sleep(1);
    }
    return NULL;
}

int main(void)
{
    char stack1[STACKSIZE];
    char stack2[STACKSIZE];

    thread1_pid = thread_create(stack1, STACKSIZE, THREAD_PRIORITY_MAIN - 1, 0, thread1_func, NULL, "thread1");
    thread2_pid = thread_create(stack2, STACKSIZE, THREAD_PRIORITY_MAIN - 2, 0, thread2_func, NULL, "thread2");

    xtimer_sleep(5);

    printf("Changing priority of thread 1\n");
    sched_change_priority(thread_get(thread1_pid), THREAD_PRIORITY_MAIN - 2);

    xtimer_sleep(5);

    printf("Changing priority of thread 2\n");
    sched_change_priority(thread_get(thread2_pid), THREAD_PRIORITY_MAIN - 1);

    return 0;
}
