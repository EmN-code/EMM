
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
#define MSG_QUEUE_SIZE 8

static char stack_low_prio[STACKSIZE];
static char stack_high_prio[STACKSIZE];

static msg_t msg_queue[MSG_QUEUE_SIZE];

void *low_prio_thread(void *arg)
{
    (void)arg;
    msg_t msg;

    while (1) {
        msg_receive(&msg);
        printf("Low priority thread received message: %d\n", msg.content.value);
    }

    return NULL;
}

void *high_prio_thread(void *arg)
{
    (void)arg;
    msg_t msg;

    while (1) {
        msg_receive(&msg);
        printf("High priority thread received message: %d\n", msg.content.value);
    }

    return NULL;
}

int main(void)
{
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    kernel_pid_t low_prio_pid = thread_create(stack_low_prio, sizeof(stack_low_prio),
                                              THREAD_PRIORITY_MAIN - 1, 0,
                                              low_prio_thread, NULL, "low_prio");

    kernel_pid_t high_prio_pid = thread_create(stack_high_prio, sizeof(stack_high_prio),
                                               THREAD_PRIORITY_MAIN - 2, 0,
                                               high_prio_thread, NULL, "high_prio");

    msg_t msg;
    msg.type = 0;

    while (1) {
        msg.content.value = rand() % 100;
        msg_send(&msg, low_prio_pid);

        msg.content.value = rand() % 100;
        msg_send(&msg, high_prio_pid);

        xtimer_sleep(1);
    }

    return 0;
}
