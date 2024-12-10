
#include "xtimer.h"
#include "thread.h"
#include "mbox.h"
#include "evtimer_msg.h"
#include "time_units.h"
#include "periph/gpio.h"
#include "event/thread.h"
#include "evtimer_mbox.h"
#include "ztimer.h"
#include "msg.h"
#include "timex.h"
#include "mma8x5x_regs.h"
#include "event/periodic_callback.h"
#include "periph/rtc.h"
#include "periph/i2c.h"
#include "MQTTClient.h"

#define STACKSIZE 512
#define NUM_MESSAGES 8

static char stack_low[STACKSIZE];
static char stack_high[STACKSIZE];

static msg_t msg_queue[NUM_MESSAGES];

static void *low_priority_thread(void *arg)
{
    (void)arg;
    msg_t msg;

    while (1) {
        msg_receive(&msg);
        printf("Low priority thread received message with value: %lu\n", (unsigned long)msg.content.value);
    }

    return NULL;
}

static void *high_priority_thread(void *arg)
{
    (void)arg;
    msg_t msg;

    while (1) {
        msg_receive(&msg);
        printf("High priority thread received message with value: %lu\n", (unsigned long)msg.content.value);
    }

    return NULL;
}

int main(void)
{
    kernel_pid_t low_pid, high_pid;
    msg_t msg;

    msg_init_queue(msg_queue, NUM_MESSAGES);

    low_pid = thread_create(stack_low, sizeof(stack_low), THREAD_PRIORITY_MAIN - 1,
                            THREAD_CREATE_STACKTEST, low_priority_thread, NULL, "low");

    high_pid = thread_create(stack_high, sizeof(stack_high), THREAD_PRIORITY_MAIN - 2,
                             THREAD_CREATE_STACKTEST, high_priority_thread, NULL, "high");

    msg.type = 0;
    msg.content.value = 1234;

    msg_send(&msg, low_pid);
    msg_send(&msg, high_pid);

    return 0;
}
