
#include "thread.h"
#include "log.h"  // Include the log header to resolve the implicit declaration errors

#define THREAD_STACKSIZE_DEFAULT (THREAD_STACKSIZE_DEFAULT)
#define THREAD_PRIORITY_LOW (THREAD_PRIORITY_MAIN - 1)
#define THREAD_PRIORITY_HIGH (THREAD_PRIORITY_MAIN)

char thread_stack_low[THREAD_STACKSIZE_DEFAULT];
char thread_stack_high[THREAD_STACKSIZE_DEFAULT];

kernel_pid_t pid_low, pid_high;

void *thread_low(void *arg)
{
    (void)arg;
    while (1) {
        log_info("Low priority thread running\n");
        thread_yield();
    }
    return NULL;
}

void *thread_high(void *arg)
{
    (void)arg;
    while (1) {
        log_info("High priority thread running\n");
        thread_yield();
    }
    return NULL;
}

int main(void)
{
    log_init();  // Initialize the logging system

    pid_low = thread_create(thread_stack_low, sizeof(thread_stack_low),
                           THREAD_PRIORITY_LOW, THREAD_CREATE_STACKTEST,
                           thread_low, NULL, "thread_low");

    pid_high = thread_create(thread_stack_high, sizeof(thread_stack_high),
                            THREAD_PRIORITY_HIGH, THREAD_CREATE_STACKTEST,
                            thread_high, NULL, "thread_high");

    // Change the priority of the low priority thread to high at runtime
    sched_change_priority(thread_get(pid_low), THREAD_PRIORITY_HIGH);

    log_info("Main thread running\n");

    while (1) {
        thread_yield();
    }

    return 0;
}
