
#include "thread.h"
#include "xtimer.h"
#include "log.h"

#define THREAD_STACKSIZE_MAIN   (THREAD_STACKSIZE_DEFAULT)
#define THREAD_PRIORITY_LOW     (THREAD_PRIORITY_MAIN - 1)
#define THREAD_PRIORITY_HIGH    (THREAD_PRIORITY_MAIN)

kernel_pid_t pid_low, pid_high;

void *thread_low_priority(void *arg)
{
    (void)arg;
    while (1) {
        LOG_INFO("Low priority thread running\n");
        xtimer_sleep(2);
    }
    return NULL;
}

void *thread_high_priority(void *arg)
{
    (void)arg;
    while (1) {
        LOG_INFO("High priority thread running\n");
        xtimer_sleep(2);
    }
    return NULL;
}

int main(void)
{
    // Initialize logging
    log_init();

    // Create low priority thread
    pid_low = thread_create(
        NULL,
        THREAD_STACKSIZE_MAIN,
        THREAD_PRIORITY_LOW,
        THREAD_CREATE_STACKTEST,
        thread_low_priority,
        NULL,
        "low_priority_thread"
    );

    // Create high priority thread
    pid_high = thread_create(
        NULL,
        THREAD_STACKSIZE_MAIN,
        THREAD_PRIORITY_HIGH,
        THREAD_CREATE_STACKTEST,
        thread_high_priority,
        NULL,
        "high_priority_thread"
    );

    // Change priority of low priority thread to high
    thread_change_priority(pid_low, THREAD_PRIORITY_HIGH);

    // Change priority of high priority thread to low
    thread_change_priority(pid_high, THREAD_PRIORITY_LOW);

    // Main thread will yield to allow other threads to run
    while (1) {
        thread_yield();
    }

    return 0;
}
