
#include "thread.h"
#include "xtimer.h"
#include "periph/gpio.h"
#include "log.h"

#define THREAD_STACKSIZE 512
#define THREAD_PRIORITY_HIGH 5
#define THREAD_PRIORITY_LOW 10

static char stack_high[THREAD_STACKSIZE];
static char stack_low[THREAD_STACKSIZE];

static kernel_pid_t pid_high, pid_low;

void *thread_high(void *arg)
{
    (void)arg;
    while (1) {
        LOG_INFO("High priority thread running\n");
        xtimer_sleep(1);
    }
    return NULL;
}

void *thread_low(void *arg)
{
    (void)arg;
    while (1) {
        LOG_INFO("Low priority thread running\n");
        xtimer_sleep(1);
    }
    return NULL;
}

int main(void)
{
    // Initialize the logging system
    log_init();

    // Create high priority thread
    pid_high = thread_create(stack_high, sizeof(stack_high),
                             THREAD_PRIORITY_HIGH, THREAD_CREATE_WOUT_YIELD,
                             thread_high, NULL, "high");

    // Create low priority thread
    pid_low = thread_create(stack_low, sizeof(stack_low),
                            THREAD_PRIORITY_LOW, THREAD_CREATE_WOUT_YIELD,
                            thread_low, NULL, "low");

    // Change priority of low priority thread to high at runtime
    thread_change_priority(pid_low, THREAD_PRIORITY_HIGH);

    // Change priority of high priority thread to low at runtime
    thread_change_priority(pid_high, THREAD_PRIORITY_LOW);

    return 0;
}
