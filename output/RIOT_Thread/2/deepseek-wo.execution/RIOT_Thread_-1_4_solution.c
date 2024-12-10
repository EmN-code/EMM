
#include "xtimer.h"
#include "thread.h"
#include "mbox.h"
#include "paho_mqtt.h"
#include "evtimer_msg.h"
#include "mma8x5x.h"
#include "time_units.h"
#include "periph/gpio.h"
#include "event/thread.h"
#include "unistd.h"
#include "evtimer_mbox.h"
#include "ztimer.h"
#include "timex.h"
#include "mma8x5x_regs.h"
#include "event/periodic_callback.h"
#include "periph/rtc.h"
#include "periph/i2c.h"
#include "MQTTClient.h"

#define STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define PRIORITY (THREAD_PRIORITY_MAIN - 1)

static char stack_high[STACKSIZE];
static char stack_low[STACKSIZE];

static void *high_priority_thread(void *arg)
{
    (void)arg;
    printf("High priority thread started\n");
    while (1) {
        // High priority thread logic
        xtimer_sleep(2);
    }
    return NULL;
}

static void *low_priority_thread(void *arg)
{
    (void)arg;
    printf("Low priority thread started\n");
    while (1) {
        // Low priority thread logic
        xtimer_sleep(5);
    }
    return NULL;
}

int main(void)
{
    kernel_pid_t pid_high, pid_low;

    // Create high priority thread
    pid_high = thread_create(stack_high, sizeof(stack_high), PRIORITY,
                             THREAD_CREATE_STACKTEST, high_priority_thread, NULL, "high");

    // Create low priority thread
    pid_low = thread_create(stack_low, sizeof(stack_low), PRIORITY - 1,
                            THREAD_CREATE_STACKTEST, low_priority_thread, NULL, "low");

    printf("Main thread started\n");

    while (1) {
        // Main thread logic
        xtimer_sleep(10);
    }

    return 0;
}
