
#include "thread.h"
#include "xtimer.h"
#include "periph/gpio.h"
#include "log.h"

#define THREAD_STACKSIZE_MAIN   (THREAD_STACKSIZE_DEFAULT)
#define THREAD_PRIORITY_HIGH    (THREAD_PRIORITY_MAIN - 1)
#define THREAD_PRIORITY_LOW     (THREAD_PRIORITY_MAIN + 1)

kernel_pid_t pid_high, pid_low;

void *thread_high(void *arg)
{
    (void)arg;
    while (1) {
        LOG_INFO("High priority thread running\n");
        xtimer_sleep(2);
    }
    return NULL;
}

void *thread_low(void *arg)
{
    (void)arg;
    while (1) {
        LOG_INFO("Low priority thread running\n");
        xtimer_sleep(2);
    }
    return NULL;
}

int main(void)
{
    // Initialize logging
    log_init();

    // Create high priority thread
    pid_high = thread_create(
        thread_stack(high_stack),
        THREAD_STACKSIZE_MAIN,
        THREAD_PRIORITY_HIGH,
        THREAD_CREATE_STACKTEST,
        thread_high,
        NULL,
        "high_thread"
    );

    // Create low priority thread
    pid_low = thread_create(
        thread_stack(low_stack),
        THREAD_STACKSIZE_MAIN,
        THREAD_PRIORITY_LOW,
        THREAD_CREATE_STACKTEST,
        thread_low,
        NULL,
        "low_thread"
    );

    // Change priority of low priority thread to high at runtime
    thread_change_priority(pid_low, THREAD_PRIORITY_HIGH);

    // Main thread will yield to allow other threads to run
    while (1) {
        thread_yield();
    }

    return 0;
}
