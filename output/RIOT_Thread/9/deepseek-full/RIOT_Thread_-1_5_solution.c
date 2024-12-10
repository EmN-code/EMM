
#include "paho_mqtt.h"
#include "ztimer.h"
#include "stddef.h"
#include "stdbool.h"
#include "msg.h"
#include "evtimer_mbox.h"
#include "timex.h"
#include "kernel_defines.h"
#include "event/thread.h"
#include "evtimer_msg.h"
#include "time_units.h"
#include "dht_params.h"
#include "stdlib.h"
#include "mbox.h"
#include "stdio.h"
#include "xtimer.h"
#include "thread.h"
#include "mma8x5x_regs.h"
#include "unistd.h"
#include "stdatomic.h"
#include "mma8x5x.h"
#include "MQTTClient.h"
#include "mma8x5x_params.h"

#define STACKSIZE 512
#define MSG_QUEUE_SIZE 16

static char stack_low_prio[STACKSIZE];
static char stack_high_prio[STACKSIZE];

static msg_t msg_queue_low_prio[MSG_QUEUE_SIZE];
static msg_t msg_queue_high_prio[MSG_QUEUE_SIZE];

void low_prio_thread(void *arg)
{
    (void)arg;
    msg_t msg;
    while (1) {
        msg_receive(&msg);
        printf("Low priority thread received message: %d\n", msg.content.value);
    }
}

void high_prio_thread(void *arg)
{
    (void)arg;
    msg_t msg;
    while (1) {
        msg_receive(&msg);
        printf("High priority thread received message: %d\n", msg.content.value);
    }
}

int main(void)
{
    msg_init_queue(msg_queue_low_prio, MSG_QUEUE_SIZE);
    msg_init_queue(msg_queue_high_prio, MSG_QUEUE_SIZE);

    kernel_pid_t low_prio_pid = thread_create(stack_low_prio, sizeof(stack_low_prio),
                                              THREAD_PRIORITY_MAIN - 1, 0, low_prio_thread, NULL, "low_prio");
    kernel_pid_t high_prio_pid = thread_create(stack_high_prio, sizeof(stack_high_prio),
                                               THREAD_PRIORITY_MAIN - 2, 0, high_prio_thread, NULL, "high_prio");

    msg_t msg;
    for (int i = 0; i < 10; i++) {
        msg.content.value = i;
        if (i % 2 == 0) {
            msg_send(&msg, high_prio_pid);
        } else {
            msg_send(&msg, low_prio_pid);
        }
        xtimer_sleep(1);
    }

    return 0;
}
