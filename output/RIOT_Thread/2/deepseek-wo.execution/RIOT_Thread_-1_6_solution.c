
#include "xtimer.h"
#include "thread.h"
#include "mbox.h"
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

static char stack[STACKSIZE];
static msg_t msg_queue[NUM_MESSAGES];

void *sender_thread(void *arg)
{
    (void)arg;
    msg_t msg;
    msg.type = 1;
    msg.content.value = 42;

    while (1) {
        msg_send_receive(&msg, &msg, thread_getpid());
        xtimer_sleep(1);
    }

    return NULL;
}

void *receiver_thread(void *arg)
{
    (void)arg;
    msg_t msg;

    while (1) {
        msg_receive(&msg);
        printf("Received message with value: %" PRIu32 "\n", msg.content.value);
    }

    return NULL;
}

int main(void)
{
    msg_init_queue(msg_queue, NUM_MESSAGES);

    kernel_pid_t sender_pid = thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                                            THREAD_CREATE_STACKTEST, sender_thread, NULL, "sender");

    kernel_pid_t receiver_pid = thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 2,
                                              THREAD_CREATE_STACKTEST, receiver_thread, NULL, "receiver");

    (void)sender_pid;
    (void)receiver_pid;

    return 0;
}
