
#include "thread.h"
#include "xtimer.h"
#include "periph/gpio.h"
#include "log.h"

#define THREAD_STACKSIZE 512
#define THREAD_PRIORITY_HIGH 5
#define THREAD_PRIORITY_LOW 10

static char stack_low[THREAD_STACKSIZE];
static char stack_high[THREAD_STACKSIZE];

static kernel_pid_t pid_low, pid_high;

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

        // Change priority of low priority thread to high
        thread_change_priority(pid_low, THREAD_PRIORITY_HIGH);
        LOG_INFO("Changed low priority thread to high priority\n");

        xtimer_sleep(10);

        // Change priority of low priority thread back to low
        thread_change_priority(pid_low, THREAD_PRIORITY_LOW);
        LOG_INFO("Changed low priority thread back to low priority\n");
    }

    return NULL;
}

int main(void)
{
    // Initialize logging
    log_init();

    // Create low priority thread
    pid_low = thread_create(stack_low, sizeof(stack_low),
                            THREAD_PRIORITY_LOW, 0,
                            thread_low_priority, NULL, "low_priority");

    // Create high priority thread
    pid_high = thread_create(stack_high, sizeof(stack_high),
                             THREAD_PRIORITY_HIGH, 0,
                             thread_high_priority, NULL, "high_priority");

    return 0;
}
