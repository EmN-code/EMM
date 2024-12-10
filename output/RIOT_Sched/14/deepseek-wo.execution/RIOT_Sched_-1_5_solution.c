
#include <stdio.h>
#include <stdlib.h>
#include <msg.h>
#include <thread.h>
#include <kernel_defines.h>

#define STACKSIZE 512
#define PRIORITY_HIGH 5
#define PRIORITY_LOW 10

kernel_pid_t pid1, pid2;

void *thread_func1(void *arg)
{
    (void)arg;
    printf("Thread 1 started with priority %d\n", thread_get_priority(thread_get_active()));
    while (1) {
        printf("Thread 1 running\n");
        thread_yield();
    }
    return NULL;
}

void *thread_func2(void *arg)
{
    (void)arg;
    printf("Thread 2 started with priority %d\n", thread_get_priority(thread_get_active()));
    while (1) {
        printf("Thread 2 running\n");
        thread_yield();
    }
    return NULL;
}

int main(void)
{
    char stack1[STACKSIZE];
    char stack2[STACKSIZE];

    pid1 = thread_create(stack1, sizeof(stack1), PRIORITY_HIGH, 0, thread_func1, NULL, "thread1");
    pid2 = thread_create(stack2, sizeof(stack2), PRIORITY_LOW, 0, thread_func2, NULL, "thread2");

    printf("Main thread started\n");

    // Change priority of thread 1 to low after some time
    xtimer_sleep(5);
    sched_change_priority(thread_get(pid1), PRIORITY_LOW);
    printf("Changed priority of Thread 1 to %d\n", PRIORITY_LOW);

    // Change priority of thread 2 to high after some time
    xtimer_sleep(5);
    sched_change_priority(thread_get(pid2), PRIORITY_HIGH);
    printf("Changed priority of Thread 2 to %d\n", PRIORITY_HIGH);

    return 0;
}
