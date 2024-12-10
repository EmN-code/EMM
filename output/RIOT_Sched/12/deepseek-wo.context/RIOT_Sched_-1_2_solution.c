
#include "thread.h"
#include "sched.h"
#include "log.h"

#define THREAD_STACKSIZE_MAIN (512)
#define THREAD_PRIORITY_LOW (THREAD_PRIORITY_MAIN - 1)
#define THREAD_PRIORITY_HIGH (THREAD_PRIORITY_MAIN - 2)

char stack_low[THREAD_STACKSIZE_MAIN];
char stack_high[THREAD_STACKSIZE_MAIN];

kernel_pid_t pid_low, pid_high;

void *thread_low_func(void *arg)
{
    (void)arg;
    while (1) {
        LOG_INFO("Low priority thread running\n");
        thread_yield();
    }
    return NULL;
}

void *thread_high_func(void *arg)
{
    (void)arg;
    while (1) {
        LOG_INFO("High priority thread running\n");
        thread_yield();
    }
    return NULL;
}

int main(void)
{
    log_init();

    pid_low = thread_create(stack_low, sizeof(stack_low), THREAD_PRIORITY_LOW,
                            THREAD_CREATE_SLEEPING, thread_low_func, NULL, "low");

    pid_high = thread_create(stack_high, sizeof(stack_high), THREAD_PRIORITY_HIGH,
                             THREAD_CREATE_SLEEPING, thread_high_func, NULL, "high");

    thread_wakeup(pid_low);
    thread_wakeup(pid_high);

    // Change priority of low priority thread to high
    sched_change_priority(pid_low, THREAD_PRIORITY_HIGH);

    // Change priority of high priority thread to low
    sched_change_priority(pid_high, THREAD_PRIORITY_LOW);

    return 0;
}
