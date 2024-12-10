
#include <stdio.h>
#include "thread.h"
#include "sched.h"

#define THREAD_STACKSIZE_DEFAULT (512)
#define THREAD_PRIORITY_MAIN (THREAD_PRIORITY_MAIN)
#define THREAD_STACKSIZE_SMALL (256)

char stack1[THREAD_STACKSIZE_DEFAULT];
char stack2[THREAD_STACKSIZE_SMALL];

kernel_pid_t pid1, pid2;

void *thread1(void *arg)
{
    (void)arg;
    printf("Thread 1 is running\n");
    return NULL;
}

void *thread2(void *arg)
{
    (void)arg;
    printf("Thread 2 is running\n");
    return NULL;
}

int main(void)
{
    pid1 = thread_create(stack1, sizeof(stack1), THREAD_PRIORITY_MAIN - 1, 0, thread1, NULL, "thread1");
    pid2 = thread_create(stack2, sizeof(stack2), THREAD_PRIORITY_MAIN - 2, 0, thread2, NULL, "thread2");

    // Change priority at runtime
    sched_change_priority(thread_get(pid1), THREAD_PRIORITY_MAIN - 3);
    sched_change_priority(thread_get(pid2), THREAD_PRIORITY_MAIN - 4);

    printf("Main thread is running\n");

    return 0;
}
