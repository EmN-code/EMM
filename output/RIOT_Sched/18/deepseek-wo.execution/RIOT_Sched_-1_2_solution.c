
#include "ztimer.h"
#include "xtimer.h"
#include "event/thread.h"
#include "timex.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "time_units.h"
#include "thread.h"

#define STACKSIZE 512

char stack1[STACKSIZE];
char stack2[STACKSIZE];

void *thread_func1(void *arg)
{
    (void)arg;
    while (1) {
        printf("Thread 1 is running\n");
        xtimer_sleep(1);
    }
    return NULL;
}

void *thread_func2(void *arg)
{
    (void)arg;
    while (1) {
        printf("Thread 2 is running\n");
        xtimer_sleep(1);
    }
    return NULL;
}

int main(void)
{
    kernel_pid_t pid1, pid2;
    uint8_t priority1, priority2;

    pid1 = thread_create(stack1, STACKSIZE, THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST, thread_func1, NULL, "thread1");
    pid2 = thread_create(stack2, STACKSIZE, THREAD_PRIORITY_MAIN - 2, THREAD_CREATE_STACKTEST, thread_func2, NULL, "thread2");

    // Get initial priorities
    priority1 = sched_active_thread->priority;
    priority2 = sched_active_thread->priority;

    printf("Initial priorities: Thread 1: %d, Thread 2: %d\n", priority1, priority2);

    // Change priorities at runtime
    sched_set_priority(pid1, THREAD_PRIORITY_MAIN - 3);
    sched_set_priority(pid2, THREAD_PRIORITY_MAIN - 4);

    // Get new priorities
    priority1 = sched_active_thread->priority;
    priority2 = sched_active_thread->priority;

    printf("New priorities: Thread 1: %d, Thread 2: %d\n", priority1, priority2);

    return 0;
}
