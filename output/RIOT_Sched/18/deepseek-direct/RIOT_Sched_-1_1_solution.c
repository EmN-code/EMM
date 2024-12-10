
#include <stdio.h>
#include "thread.h"
#include "xtimer.h"

#define THREAD_STACKSIZE 512

char thread_stack1[THREAD_STACKSIZE];
char thread_stack2[THREAD_STACKSIZE];

kernel_pid_t thread_id1, thread_id2;

void *thread_func1(void *arg)
{
    (void)arg;
    while (1) {
        printf("Thread 1 running at priority %d\n", sched_active_thread->priority);
        xtimer_sleep(2);
    }
    return NULL;
}

void *thread_func2(void *arg)
{
    (void)arg;
    while (1) {
        printf("Thread 2 running at priority %d\n", sched_active_thread->priority);
        xtimer_sleep(2);
    }
    return NULL;
}

int main(void)
{
    puts("RIOT application starting on ESP32");

    // Create the first thread with priority 3
    thread_id1 = thread_create(thread_stack1, sizeof(thread_stack1), 3,
                               THREAD_CREATE_STACKTEST, thread_func1, NULL, "thread1");

    // Create the second thread with priority 4
    thread_id2 = thread_create(thread_stack2, sizeof(thread_stack2), 4,
                               THREAD_CREATE_STACKTEST, thread_func2, NULL, "thread2");

    // Change the priority of thread 1 to 5 after 10 seconds
    xtimer_sleep(10);
    thread_change_priority(thread_id1, 5);
    printf("Changed priority of thread 1 to 5\n");

    // Change the priority of thread 2 to 2 after 20 seconds
    xtimer_sleep(10);
    thread_change_priority(thread_id2, 2);
    printf("Changed priority of thread 2 to 2\n");

    return 0;
}
