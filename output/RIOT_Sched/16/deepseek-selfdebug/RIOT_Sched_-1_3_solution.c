
#include <stdio.h>
#include "thread.h"
#include "xtimer.h"

#ifndef THREAD_STACKSIZE_DEFAULT
#define THREAD_STACKSIZE_DEFAULT (512)
#endif

#define THREAD_PRIORITY_HIGH (THREAD_PRIORITY_MAIN - 1)
#define THREAD_PRIORITY_LOW (THREAD_PRIORITY_MAIN + 1)

char stack_high[THREAD_STACKSIZE_DEFAULT];
char stack_low[THREAD_STACKSIZE_DEFAULT];

kernel_pid_t pid_high, pid_low;

void *thread_high(void *arg)
{
    (void)arg;
    while (1) {
        printf("High priority thread running\n");
        xtimer_sleep(2);
    }
    return NULL;
}

void *thread_low(void *arg)
{
    (void)arg;
    while (1) {
        printf("Low priority thread running\n");
        xtimer_sleep(2);
    }
    return NULL;
}

int main(void)
{
    puts("Starting two threads with different priorities");

    pid_high = thread_create(stack_high, sizeof(stack_high), THREAD_PRIORITY_HIGH,
                             THREAD_CREATE_STACKTEST, thread_high, NULL, "high");

    pid_low = thread_create(stack_low, sizeof(stack_low), THREAD_PRIORITY_LOW,
                            THREAD_CREATE_STACKTEST, thread_low, NULL, "low");

    xtimer_sleep(10);

    puts("Changing priority of low priority thread to high");
    sched_change_priority(pid_low, THREAD_PRIORITY_HIGH);

    xtimer_sleep(10);

    puts("Changing priority of high priority thread to low");
    sched_change_priority(pid_high, THREAD_PRIORITY_LOW);

    return 0;
}
