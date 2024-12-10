
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
#define NUM_MESSAGES 16

static char stack_high[STACKSIZE];
static char stack_low[STACKSIZE];

static msg_t msg_queue[NUM_MESSAGES];

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

int main(void)
{
    kernel_pid_t high_pid, low_pid;
    msg_t msg;

    msg_init_queue(msg_queue, NUM_MESSAGES);

    high_pid = thread_create(stack_high, sizeof(stack_high), THREAD_PRIORITY_MAIN - 1, 0, high_priority_thread, NULL, "high");
    low_pid = thread_create(stack_low, sizeof(stack_low), THREAD_PRIORITY_MAIN - 2, 0, low_priority_thread, NULL, "low");

    msg.type = 0;

    for (uint32_t i = 0; i < 10; i++) {
        msg.content.value = i;
        msg_send(&msg, (i % 2 == 0) ? high_pid : low_pid);
        xtimer_sleep(1);
    }

    return 0;
}
