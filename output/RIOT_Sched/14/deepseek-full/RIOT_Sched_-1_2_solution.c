
#include <stdio.h>
#include "evtimer_mbox.h"
#include "xtimer.h"
#include "timex.h"
#include "thread.h"
#include "ztimer.h"
#include "event/thread.h"
#include "evtimer_msg.h"
#include "time_units.h"

#define THREAD_STACKSIZE 512
#define PRIORITY_HIGH 5
#define PRIORITY_LOW 10

char stack_high[THREAD_STACKSIZE];
char stack_low[THREAD_STACKSIZE];

void *thread_high(void *arg)
{
    (void)arg;
    printf("High priority thread running\n");
    return NULL;
}

void *thread_low(void *arg)
{
    (void)arg;
    printf("Low priority thread running\n");
    return NULL;
}

int main(void)
{
    kernel_pid_t pid_high, pid_low;

    pid_high = thread_create(stack_high, sizeof(stack_high), PRIORITY_HIGH, 0, thread_high, NULL, "high_thread");
    pid_low = thread_create(stack_low, sizeof(stack_low), PRIORITY_LOW, 0, thread_low, NULL, "low_thread");

    xtimer_sleep(2); // Let threads run for 2 seconds

    printf("Changing priority of high priority thread to low\n");
    sched_change_priority(pid_high, PRIORITY_LOW);

    xtimer_sleep(2); // Let threads run for another 2 seconds

    printf("Changing priority of low priority thread to high\n");
    sched_change_priority(pid_low, PRIORITY_HIGH);

    xtimer_sleep(2); // Let threads run for another 2 seconds

    return 0;
}
