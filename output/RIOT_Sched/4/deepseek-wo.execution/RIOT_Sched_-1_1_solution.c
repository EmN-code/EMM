
#include <stdio.h>
#include "thread.h"
#include "xtimer.h"

#define THREAD_STACKSIZE 512
#define THREAD_PRIORITY_HIGH (THREAD_PRIORITY_MAIN - 1)
#define THREAD_PRIORITY_LOW (THREAD_PRIORITY_MAIN + 1)

kernel_pid_t thread_pid_1, thread_pid_2;
char thread_stack_1[THREAD_STACKSIZE];
char thread_stack_2[THREAD_STACKSIZE];

void *thread_func_1(void *arg)
{
    (void)arg;
    while (1) {
        printf("Thread 1 running at priority %d\n", thread_get_priority(thread_pid_1));
        xtimer_sleep(2);
    }
    return NULL;
}

void *thread_func_2(void *arg)
{
    (void)arg;
    while (1) {
        printf("Thread 2 running at priority %d\n", thread_get_priority(thread_pid_2));
        xtimer_sleep(2);
    }
    return NULL;
}

int main(void)
{
    // Create the first thread with low priority
    thread_pid_1 = thread_create(thread_stack_1, sizeof(thread_stack_1),
                                 THREAD_PRIORITY_LOW, THREAD_CREATE_STACKTEST,
                                 thread_func_1, NULL, "thread_1");

    // Create the second thread with high priority
    thread_pid_2 = thread_create(thread_stack_2, sizeof(thread_stack_2),
                                 THREAD_PRIORITY_HIGH, THREAD_CREATE_STACKTEST,
                                 thread_func_2, NULL, "thread_2");

    // Change the priority of the first thread to high after 10 seconds
    xtimer_sleep(10);
    sched_change_priority(thread_pid_1, THREAD_PRIORITY_HIGH);
    printf("Changed priority of Thread 1 to HIGH\n");

    // Change the priority of the second thread to low after 20 seconds
    xtimer_sleep(10);
    sched_change_priority(thread_pid_2, THREAD_PRIORITY_LOW);
    printf("Changed priority of Thread 2 to LOW\n");

    return 0;
}
