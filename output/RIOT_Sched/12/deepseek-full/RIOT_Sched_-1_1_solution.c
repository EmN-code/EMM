
#include <stdio.h>
#include "thread.h"
#include "xtimer.h"

#define THREAD_STACKSIZE 512
#define THREAD1_PRIO (THREAD_PRIORITY_MAIN - 1)
#define THREAD2_PRIO (THREAD_PRIORITY_MAIN - 2)

kernel_pid_t thread1_pid, thread2_pid;
char thread1_stack[THREAD_STACKSIZE];
char thread2_stack[THREAD_STACKSIZE];

void *thread1_func(void *arg)
{
    (void)arg;
    printf("Thread 1 started with priority %d\n", thread_get_priority(thread_getpid()));

    // Change priority of Thread 2 to a higher priority
    sched_change_priority(thread2_pid, THREAD1_PRIO);
    printf("Changed Thread 2 priority to %d\n", THREAD1_PRIO);

    while (1) {
        printf("Thread 1 running\n");
        xtimer_sleep(2);
    }
    return NULL;
}

void *thread2_func(void *arg)
{
    (void)arg;
    printf("Thread 2 started with priority %d\n", thread_get_priority(thread_getpid()));

    while (1) {
        printf("Thread 2 running\n");
        xtimer_sleep(2);
    }
    return NULL;
}

int main(void)
{
    printf("Starting RIOT application on ESP32\n");

    // Create and start Thread 1
    thread1_pid = thread_create(thread1_stack, sizeof(thread1_stack), THREAD1_PRIO,
                                THREAD_CREATE_STACKTEST, thread1_func, NULL, "thread1");

    // Create and start Thread 2
    thread2_pid = thread_create(thread2_stack, sizeof(thread2_stack), THREAD2_PRIO,
                                THREAD_CREATE_STACKTEST, thread2_func, NULL, "thread2");

    // Main thread will just yield to allow other threads to run
    while (1) {
        thread_yield();
    }

    return 0;
}
