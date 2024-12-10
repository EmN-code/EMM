
#include "xtimer.h"
#include "thread.h"
#include "mbox.h"
#include "string.h"
#include "kernel_defines.h"
#include "stddef.h"
#include "stdatomic.h"
#include "paho_mqtt.h"
#include "evtimer_msg.h"
#include "mma8x5x.h"
#include "time_units.h"
#include "periph/gpio.h"
#include "event/thread.h"
#include "unistd.h"
#include "stdlib.h"
#include "evtimer_mbox.h"
#include "ztimer.h"
#include "msg.h"
#include "timex.h"
#include "mma8x5x_regs.h"
#include "stdbool.h"
#include "event/periodic_callback.h"
#include "periph/rtc.h"
#include "stdio.h"
#include "periph/i2c.h"
#include "MQTTClient.h"

#define STACKSIZE 512
#define NUM_MESSAGES 16

static char stack_high[STACKSIZE];
static char stack_low[STACKSIZE];

static msg_t msg_queue[NUM_MESSAGES];

void *high_priority_thread(void *arg)
{
    (void)arg;
    msg_t msg;
    while (1) {
        msg_receive(&msg);
        printf("High priority thread received message with value: %lu\n", (unsigned long)msg.content.value);
    }
    return NULL;
}

void *low_priority_thread(void *arg)
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
    msg_init_queue(msg_queue, NUM_MESSAGES);

    kernel_pid_t pid_high = thread_create(stack_high, sizeof(stack_high), THREAD_PRIORITY_MAIN + 1,
                                          THREAD_CREATE_STACKTEST, high_priority_thread, NULL, "high");

    kernel_pid_t pid_low = thread_create(stack_low, sizeof(stack_low), THREAD_PRIORITY_MAIN - 1,
                                         THREAD_CREATE_STACKTEST, low_priority_thread, NULL, "low");

    msg_t msg;
    msg.type = 0;

    for (uint32_t i = 0; i < 10; i++) {
        msg.content.value = i;
        if (i % 2 == 0) {
            msg_send(&msg, pid_high);
        } else {
            msg_send(&msg, pid_low);
        }
        xtimer_sleep(1);
    }

    return 0;
}
