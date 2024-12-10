
#include <stdio.h>
#include "thread.h"
#include "sched.h"

#define THREAD_STACKSIZE_DEFAULT (512)

kernel_pid_t thread1_pid, thread2_pid;
char thread1_stack[THREAD_STACKSIZE_DEFAULT];
char thread2_stack[THREAD_STACKSIZE_DEFAULT];

void *thread1_func(void *arg)
{
    (void)arg;
    printf("Thread 1 running\n");
    return NULL;
}

void *thread2_func(void *arg)
{
    (void)arg;
    printf("Thread 2 running\n");
    return NULL;
}

int main(void)
{
    // Create the first thread
    thread1_pid = thread_create(thread1_stack, sizeof(thread1_stack),
                                THREAD_PRIORITY_MAIN - 1,
                                THREAD_CREATE_STACKTEST,
                                thread1_func, NULL, "thread1");

    // Create the second thread
    thread2_pid = thread_create(thread2_stack, sizeof(thread2_stack),
                                THREAD_PRIORITY_MAIN - 2,
                                THREAD_CREATE_STACKTEST,
                                thread2_func, NULL, "thread2");

    // Change the priority of the first thread at runtime
    thread_t *thread1 = thread_get(thread1_pid);
    sched_change_priority(thread1, THREAD_PRIORITY_MAIN - 3);

    // Change the priority of the second thread at runtime
    thread_t *thread2 = thread_get(thread2_pid);
    sched_change_priority(thread2, THREAD_PRIORITY_MAIN - 4);

    return 0;
}
