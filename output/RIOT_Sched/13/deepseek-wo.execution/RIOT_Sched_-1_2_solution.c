
#include "thread.h"
#include "msg.h"
#include "xtimer.h"
#include <stdio.h>

#define STACKSIZE 512
#define PRIORITY 5

kernel_pid_t thread1_pid, thread2_pid;

void *thread1_func(void *arg)
{
    (void)arg;
    printf("Thread 1 started\n");
    xtimer_sleep(5);
    printf("Thread 1 changing priority\n");
    sched_change_priority(thread_get_active(), 3);
    xtimer_sleep(5);
    printf("Thread 1 finished\n");
    return NULL;
}

void *thread2_func(void *arg)
{
    (void)arg;
    printf("Thread 2 started\n");
    xtimer_sleep(5);
    printf("Thread 2 changing priority\n");
    sched_change_priority(thread_get_active(), 7);
    xtimer_sleep(5);
    printf("Thread 2 finished\n");
    return NULL;
}

int main(void)
{
    char stack1[STACKSIZE];
    char stack2[STACKSIZE];

    thread1_pid = thread_create(stack1, sizeof(stack1), PRIORITY, 0, thread1_func, NULL, "thread1");
    thread2_pid = thread_create(stack2, sizeof(stack2), PRIORITY, 0, thread2_func, NULL, "thread2");

    printf("Main thread finished\n");
    return 0;
}
