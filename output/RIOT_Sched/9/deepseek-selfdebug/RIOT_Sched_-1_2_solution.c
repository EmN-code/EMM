
#include <stdio.h>
#include <thread.h>
#include <xtimer.h>

#define THREAD_STACKSIZE 512
#define THREAD_PRIORITY THREAD_PRIORITY_MAIN - 1

kernel_pid_t thread1_pid, thread2_pid;

void *thread1_func(void *arg)
{
    (void)arg;
    while (1) {
        printf("Thread 1 is running\n");
        xtimer_sleep(2);
    }
    return NULL;
}

void *thread2_func(void *arg)
{
    (void)arg;
    while (1) {
        printf("Thread 2 is running\n");
        xtimer_sleep(2);
    }
    return NULL;
}

int main(void)
{
    thread1_pid = thread_create(
        (char *)malloc(THREAD_STACKSIZE),
        THREAD_STACKSIZE,
        THREAD_PRIORITY,
        THREAD_CREATE_STACKTEST,
        thread1_func,
        NULL,
        "thread1"
    );

    thread2_pid = thread_create(
        (char *)malloc(THREAD_STACKSIZE),
        THREAD_STACKSIZE,
        THREAD_PRIORITY,
        THREAD_CREATE_STACKTEST,
        thread2_func,
        NULL,
        "thread2"
    );

    // Change priority of thread1 to a higher priority
    sched_change_priority(thread1_pid, THREAD_PRIORITY_MAIN - 2);

    // Change priority of thread2 to a lower priority
    sched_change_priority(thread2_pid, THREAD_PRIORITY_MAIN);

    while (1) {
        printf("Main thread is running\n");
        xtimer_sleep(5);
    }

    return 0;
}
