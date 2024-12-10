
#include <stdio.h>
#include <thread.h>
#include <ztimer.h>
#include <event/thread.h>
#include <evtimer_msg.h>
#include <timex.h>
#include <time_units.h>

#define STACKSIZE 1024
#define PRIORITY_HIGH 5
#define PRIORITY_LOW 10

kernel_pid_t thread_high, thread_low;

void *thread_high_func(void *arg)
{
    (void)arg;
    printf("High priority thread running\n");
    return NULL;
}

void *thread_low_func(void *arg)
{
    (void)arg;
    printf("Low priority thread running\n");
    return NULL;
}

int main(void)
{
    char stack_high[STACKSIZE];
    char stack_low[STACKSIZE];

    thread_high = thread_create(stack_high, sizeof(stack_high), PRIORITY_HIGH,
                                THREAD_CREATE_STACKTEST, thread_high_func, NULL, "thread_high");

    thread_low = thread_create(stack_low, sizeof(stack_low), PRIORITY_LOW,
                               THREAD_CREATE_STACKTEST, thread_low_func, NULL, "thread_low");

    // Change priority of thread_low to high
    sched_change_priority(thread_get(thread_low), PRIORITY_HIGH);

    // Change priority of thread_high to low
    sched_change_priority(thread_get(thread_high), PRIORITY_LOW);

    return 0;
}
