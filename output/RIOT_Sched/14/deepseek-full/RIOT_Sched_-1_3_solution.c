
#include "sched.h"
#include "thread.h"
#include "timex.h"
#include "ztimer.h"
#include "mbox.h"
#include "evtimer_msg.h"
#include "time_units.h"

#define STACKSIZE 512
#define PRIORITY_HIGH 7
#define PRIORITY_LOW 15

kernel_pid_t pid_high, pid_low;

void *high_priority_thread(void *arg)
{
    (void)arg;
    while (1) {
        printf("High priority thread running\n");
        ztimer_sleep(ZTIMER_MSEC, 2000);
    }
    return NULL;
}

void *low_priority_thread(void *arg)
{
    (void)arg;
    while (1) {
        printf("Low priority thread running\n");
        ztimer_sleep(ZTIMER_MSEC, 2000);
    }
    return NULL;
}

int main(void)
{
    char stack_high[STACKSIZE];
    char stack_low[STACKSIZE];

    pid_high = thread_create(stack_high, STACKSIZE, PRIORITY_HIGH, 0, high_priority_thread, NULL, "high_prio");
    pid_low = thread_create(stack_low, STACKSIZE, PRIORITY_LOW, 0, low_priority_thread, NULL, "low_prio");

    if (pid_high == KERNEL_PID_UNDEF || pid_low == KERNEL_PID_UNDEF) {
        printf("Error: Failed to create threads\n");
        return -1;
    }

    ztimer_sleep(ZTIMER_MSEC, 5000);

    printf("Changing priority of high priority thread to low\n");
    sched_change_priority(pid_high, PRIORITY_LOW);

    ztimer_sleep(ZTIMER_MSEC, 5000);

    printf("Changing priority of low priority thread to high\n");
    sched_change_priority(pid_low, PRIORITY_HIGH);

    return 0;
}
