
#include <stdio.h>
#include "thread.h"
#include "xtimer.h"

#define THREAD_STACKSIZE_DEFAULT (THREAD_STACKSIZE_MAIN)

kernel_pid_t thread1_pid, thread2_pid;

void *thread1_func(void *arg)
{
    (void)arg;
    while (1) {
        printf("Thread 1 running at priority %d\n", thread_get_priority(thread_getpid()));
        xtimer_sleep(2);
    }
    return NULL;
}

void *thread2_func(void *arg)
{
    (void)arg;
    while (1) {
        printf("Thread 2 running at priority %d\n", thread_get_priority(thread_getpid()));
        xtimer_sleep(2);
    }
    return NULL;
}

int main(void)
{
    puts("Starting two threads with different priorities...");

    // Create and start thread 1 with a lower priority
    thread1_pid = thread_create(
        thread_stack, THREAD_STACKSIZE_DEFAULT,
        THREAD_PRIORITY_MAIN - 1,
        THREAD_CREATE_STACKTEST,
        thread1_func, NULL, "thread1");

    // Create and start thread 2 with a higher priority
    thread2_pid = thread_create(
        thread_stack, THREAD_STACKSIZE_DEFAULT,
        THREAD_PRIORITY_MAIN - 2,
        THREAD_CREATE_STACKTEST,
        thread2_func, NULL, "thread2");

    // Change the priority of thread 1 to a higher priority after 10 seconds
    xtimer_sleep(10);
    sched_change_priority(thread_get(thread1_pid), THREAD_PRIORITY_MAIN - 2);
    printf("Changed priority of thread 1 to %d\n", THREAD_PRIORITY_MAIN - 2);

    // Change the priority of thread 2 to a lower priority after 20 seconds
    xtimer_sleep(10);
    sched_change_priority(thread_get(thread2_pid), THREAD_PRIORITY_MAIN - 1);
    printf("Changed priority of thread 2 to %d\n", THREAD_PRIORITY_MAIN - 1);

    return 0;
}
