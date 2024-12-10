
#include "thread.h"
#include "msg.h"
#include "stdio.h"

#define THREAD_STACKSIZE_DEFAULT (THREAD_STACKSIZE_DEFAULT + DEBUG_EXTRA_STACKSIZE)

kernel_pid_t thread1_pid, thread2_pid;

void *thread1_func(void *arg)
{
    (void)arg;
    while (1) {
        printf("Thread 1 running with priority: %d\n", (int)thread_get_priority(thread_get_pid()));
        thread_yield();
    }
    return NULL;
}

void *thread2_func(void *arg)
{
    (void)arg;
    while (1) {
        printf("Thread 2 running with priority: %d\n", (int)thread_get_priority(thread_get_pid()));
        thread_yield();
    }
    return NULL;
}

int main(void)
{
    thread1_pid = thread_create(
        (char *)malloc(THREAD_STACKSIZE_DEFAULT),
        THREAD_STACKSIZE_DEFAULT,
        THREAD_PRIORITY_MAIN - 1,
        THREAD_CREATE_STACKTEST,
        thread1_func,
        NULL,
        "thread1"
    );

    thread2_pid = thread_create(
        (char *)malloc(THREAD_STACKSIZE_DEFAULT),
        THREAD_STACKSIZE_DEFAULT,
        THREAD_PRIORITY_MAIN - 2,
        THREAD_CREATE_STACKTEST,
        thread2_func,
        NULL,
        "thread2"
    );

    // Change priority of thread1 to a higher priority
    sched_change_priority(thread_get(thread1_pid), THREAD_PRIORITY_MAIN - 3);

    // Change priority of thread2 to a lower priority
    sched_change_priority(thread_get(thread2_pid), THREAD_PRIORITY_MAIN - 4);

    return 0;
}
