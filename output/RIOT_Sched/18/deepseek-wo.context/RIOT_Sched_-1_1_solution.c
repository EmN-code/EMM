
#include <stdio.h>
#include "thread.h"
#include "xtimer.h"

#define THREAD_STACKSIZE 512
#define THREAD1_PRIO (THREAD_PRIORITY_MAIN - 1)
#define THREAD2_PRIO (THREAD_PRIORITY_MAIN - 2)

char thread1_stack[THREAD_STACKSIZE];
char thread2_stack[THREAD_STACKSIZE];

kernel_pid_t thread1_pid;
kernel_pid_t thread2_pid;

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
    // Create and start the first thread
    thread1_pid = thread_create(thread1_stack, sizeof(thread1_stack), THREAD1_PRIO,
                                THREAD_CREATE_STACKTEST, thread1_func, NULL, "thread1");

    // Create and start the second thread
    thread2_pid = thread_create(thread2_stack, sizeof(thread2_stack), THREAD2_PRIO,
                                THREAD_CREATE_STACKTEST, thread2_func, NULL, "thread2");

    // Change the priority of thread1 to a lower priority
    thread_change_priority(thread1_pid, THREAD2_PRIO);

    // Change the priority of thread2 to a higher priority
    thread_change_priority(thread2_pid, THREAD1_PRIO);

    // Main thread will also run and print its priority
    while (1) {
        printf("Main thread running at priority %d\n", thread_get_priority(thread_getpid()));
        xtimer_sleep(2);
    }

    return 0;
}
