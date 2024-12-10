
#include "thread.h"
#include "xtimer.h"
#include "periph/gpio.h"
#include "log.h"

#define THREAD_STACKSIZE_MAIN   (THREAD_STACKSIZE_DEFAULT)
#define THREAD_PRIORITY_LOW     (THREAD_PRIORITY_MAIN - 1)
#define THREAD_PRIORITY_HIGH    (THREAD_PRIORITY_MAIN)

kernel_pid_t thread_low_pid;
kernel_pid_t thread_high_pid;

void *thread_low_func(void *arg)
{
    (void)arg;
    while (1) {
        LOG_INFO("Low priority thread running\n");
        xtimer_sleep(2);
    }
    return NULL;
}

void *thread_high_func(void *arg)
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
    thread_low_pid = thread_create(
        thread_stack_t(thread_low),
        THREAD_STACKSIZE_MAIN,
        THREAD_PRIORITY_LOW,
        THREAD_CREATE_STACKTEST,
        thread_low_func,
        NULL,
        "thread_low"
    );

    // Create high priority thread
    thread_high_pid = thread_create(
        thread_stack_t(thread_high),
        THREAD_STACKSIZE_MAIN,
        THREAD_PRIORITY_HIGH,
        THREAD_CREATE_STACKTEST,
        thread_high_func,
        NULL,
        "thread_high"
    );

    // Change priority of low priority thread to high at runtime
    thread_change_priority(thread_low_pid, THREAD_PRIORITY_HIGH);

    // Change priority of high priority thread to low at runtime
    thread_change_priority(thread_high_pid, THREAD_PRIORITY_LOW);

    // Main thread will yield to allow other threads to run
    while (1) {
        thread_yield();
    }

    return 0;
}
