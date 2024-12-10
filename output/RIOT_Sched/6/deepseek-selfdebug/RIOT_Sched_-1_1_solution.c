
#include "thread.h"
#include "xtimer.h"
#include "periph/gpio.h"
#include "log.h"

#define THREAD_STACKSIZE_MAIN   (THREAD_STACKSIZE_DEFAULT)
#define THREAD_PRIORITY_HIGH    (THREAD_PRIORITY_MAIN - 1)
#define THREAD_PRIORITY_LOW     (THREAD_PRIORITY_MAIN + 1)

kernel_pid_t high_prio_thread_pid;
kernel_pid_t low_prio_thread_pid;

void *high_prio_thread(void *arg)
{
    (void)arg;
    while (1) {
        LOG_INFO("High priority thread running\n");
        xtimer_sleep(2);
    }
    return NULL;
}

void *low_prio_thread(void *arg)
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
    high_prio_thread_pid = thread_create(
        thread_stack_init(THREAD_STACKSIZE_MAIN),
        THREAD_PRIORITY_HIGH,
        THREAD_CREATE_STACKTEST,
        high_prio_thread,
        NULL,
        "high_prio_thread"
    );

    // Create low priority thread
    low_prio_thread_pid = thread_create(
        thread_stack_init(THREAD_STACKSIZE_MAIN),
        THREAD_PRIORITY_LOW,
        THREAD_CREATE_STACKTEST,
        low_prio_thread,
        NULL,
        "low_prio_thread"
    );

    // Change priority of low priority thread to high at runtime
    thread_change_priority(low_prio_thread_pid, THREAD_PRIORITY_HIGH);

    // Main thread will yield to allow other threads to run
    while (1) {
        thread_yield();
    }

    return 0;
}
