
#include <stdio.h>
#include "thread.h"
#include "sched.h"

#define THREAD_STACKSIZE_DEFAULT (THREAD_STACKSIZE_MAIN)
#define THREAD_PRIORITY_DEFAULT (THREAD_PRIORITY_MAIN - 1)

char thread1_stack[THREAD_STACKSIZE_DEFAULT];
char thread2_stack[THREAD_STACKSIZE_DEFAULT];

void *thread1_func(void *arg)
{
    (void)arg;
    printf("Thread 1 is running\n");
    return NULL;
}

void *thread2_func(void *arg)
{
    (void)arg;
    printf("Thread 2 is running\n");
    return NULL;
}

int main(void)
{
    kernel_pid_t pid1, pid2;
    thread_t *thread1, *thread2;

    // Create the first thread
    pid1 = thread_create(thread1_stack, sizeof(thread1_stack), THREAD_PRIORITY_DEFAULT,
                         THREAD_CREATE_STACKTEST, thread1_func, NULL, "thread1");
    thread1 = thread_get(pid1);

    // Create the second thread
    pid2 = thread_create(thread2_stack, sizeof(thread2_stack), THREAD_PRIORITY_DEFAULT,
                         THREAD_CREATE_STACKTEST, thread2_func, NULL, "thread2");
    thread2 = thread_get(pid2);

    // Change the priority of the first thread
    uint8_t old_priority1 = thread_get_priority(thread1);
    sched_change_priority(thread1, THREAD_PRIORITY_MAIN - 2);
    printf("Changed priority of thread1 from %u to %u\n", old_priority1, thread_get_priority(thread1));

    // Change the priority of the second thread
    uint8_t old_priority2 = thread_get_priority(thread2);
    sched_change_priority(thread2, THREAD_PRIORITY_MAIN - 3);
    printf("Changed priority of thread2 from %u to %u\n", old_priority2, thread_get_priority(thread2));

    return 0;
}
