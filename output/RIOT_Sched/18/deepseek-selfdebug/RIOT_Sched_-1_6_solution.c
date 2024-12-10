
#include <stdio.h>
#include "thread.h"
#include "sched.h"

#define THREAD_STACKSIZE_DEFAULT (512)

char stack1[THREAD_STACKSIZE_DEFAULT];
char stack2[THREAD_STACKSIZE_DEFAULT];

kernel_pid_t pid1, pid2;

void *thread1(void *arg)
{
    (void)arg;
    while (1) {
        printf("Thread 1 running\n");
        thread_yield();
    }
    return NULL;
}

void *thread2(void *arg)
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
    pid1 = thread_create(stack1, sizeof(stack1), THREAD_PRIORITY_MAIN - 1, 0, thread1, NULL, "thread1");
    pid2 = thread_create(stack2, sizeof(stack2), THREAD_PRIORITY_MAIN - 1, 0, thread2, NULL, "thread2");

    // Change priority of thread1 and thread2 at runtime
    thread_t *thread1_ptr = thread_get(pid1);
    thread_t *thread2_ptr = thread_get(pid2);

    sched_change_priority(thread1_ptr, THREAD_PRIORITY_MAIN - 2);
    sched_change_priority(thread2_ptr, THREAD_PRIORITY_MAIN - 2);

    return 0;
}
