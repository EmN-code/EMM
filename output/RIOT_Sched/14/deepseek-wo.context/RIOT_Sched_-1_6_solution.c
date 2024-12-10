
#include "thread.h"
#include "xtimer.h"
#include "msg.h"

#define THREAD_STACKSIZE_DEFAULT (THREAD_STACKSIZE_DEFAULT + DEBUG_EXTRA_STACKSIZE)
#define THREAD_PRIORITY_LOW (THREAD_PRIORITY_MAIN - 1)
#define THREAD_PRIORITY_HIGH (THREAD_PRIORITY_MAIN - 2)

char stack_low[THREAD_STACKSIZE_DEFAULT];
char stack_high[THREAD_STACKSIZE_DEFAULT];

kernel_pid_t pid_low, pid_high;

void *thread_low(void *arg)
{
    (void)arg;
    while (1) {
        printf("Low priority thread running\n");
        xtimer_sleep(2);
    }
    return NULL;
}

void *thread_high(void *arg)
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
    pid_low = thread_create(stack_low, sizeof(stack_low), THREAD_PRIORITY_LOW,
                            THREAD_CREATE_STACKTEST, thread_low, NULL, "low");

    pid_high = thread_create(stack_high, sizeof(stack_high), THREAD_PRIORITY_HIGH,
                             THREAD_CREATE_STACKTEST, thread_high, NULL, "high");

    xtimer_sleep(10);

    printf("Changing priority of low thread to high\n");
    sched_change_priority(pid_low, THREAD_PRIORITY_HIGH);

    xtimer_sleep(10);

    printf("Changing priority of high thread to low\n");
    sched_change_priority(pid_high, THREAD_PRIORITY_LOW);

    return 0;
}
