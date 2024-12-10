
#include <stdio.h>
#include "thread.h"
#include "sched.h"

#define THREAD_STACKSIZE_DEFAULT (512)

char thread1_stack[THREAD_STACKSIZE_DEFAULT];
char thread2_stack[THREAD_STACKSIZE_DEFAULT];

kernel_pid_t thread1_pid;
kernel_pid_t thread2_pid;

void *thread1_func(void *arg)
{
    (void)arg;
    while (1) {
        printf("Thread 1 running\n");
        thread_yield();
    }
    return NULL;
}

void *thread2_func(void *arg)
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
    thread1_pid = thread_create(thread1_stack, sizeof(thread1_stack),
                                THREAD_PRIORITY_MAIN - 1,
                                THREAD_CREATE_STACKTEST,
                                thread1_func, NULL, "thread1");

    thread2_pid = thread_create(thread2_stack, sizeof(thread2_stack),
                                THREAD_PRIORITY_MAIN - 2,
                                THREAD_CREATE_STACKTEST,
                                thread2_func, NULL, "thread2");

    // Change priority at runtime
    thread_t *thread1 = thread_get(thread1_pid);
    thread_t *thread2 = thread_get(thread2_pid);

    sched_change_priority(thread1, THREAD_PRIORITY_MAIN - 3);
    sched_change_priority(thread2, THREAD_PRIORITY_MAIN - 4);

    return 0;
}
