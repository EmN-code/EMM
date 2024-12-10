
#include <stdio.h>
#include "thread.h"
#include "xtimer.h"

#define THREAD_STACKSIZE_MAIN   (THREAD_STACKSIZE_DEFAULT)
#define THREAD_PRIORITY_LOW     (THREAD_PRIORITY_MAIN - 1)
#define THREAD_PRIORITY_HIGH    (THREAD_PRIORITY_MAIN - 2)

kernel_pid_t thread_low_pid;
kernel_pid_t thread_high_pid;

void *thread_low_func(void *arg)
{
    (void)arg;
    while (1) {
        printf("Low priority thread running\n");
        xtimer_sleep(2);
    }
    return NULL;
}

void *thread_high_func(void *arg)
{
    (void)arg;
    while (1) {
        printf("High priority thread running\n");
        xtimer_sleep(2);
    }
    return NULL;
}

int main(void)
{
    puts("Starting two threads with different priorities and changing priority at runtime");

    // Create low priority thread
    thread_low_pid = thread_create(
        thread_stack,
        THREAD_STACKSIZE_MAIN,
        THREAD_PRIORITY_LOW,
        THREAD_CREATE_STACKTEST,
        thread_low_func,
        NULL,
        "thread_low"
    );

    // Create high priority thread
    thread_high_pid = thread_create(
        thread_stack,
        THREAD_STACKSIZE_MAIN,
        THREAD_PRIORITY_HIGH,
        THREAD_CREATE_STACKTEST,
        thread_high_func,
        NULL,
        "thread_high"
    );

    // Change priority of low priority thread to high
    sched_change_priority(thread_low_pid, THREAD_PRIORITY_HIGH);
    printf("Changed priority of low priority thread to high\n");

    // Change priority of high priority thread to low
    sched_change_priority(thread_high_pid, THREAD_PRIORITY_LOW);
    printf("Changed priority of high priority thread to low\n");

    return 0;
}
