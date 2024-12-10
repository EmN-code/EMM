
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "paho_mqtt.h"
#include "ztimer.h"
#include "msg.h"
#include "evtimer_mbox.h"
#include "timex.h"
#include "event/thread.h"
#include "evtimer_msg.h"
#include "time_units.h"
#include "mbox.h"
#include "xtimer.h"
#include "thread.h"
#include "mma8x5x_regs.h"
#include "mma8x5x.h"
#include "mma8x5x_params.h"

#define STACKSIZE 1024
#define MSG_QUEUE_SIZE 16

static char stack_high[STACKSIZE];
static char stack_low[STACKSIZE];

static msg_t msg_queue[MSG_QUEUE_SIZE];

void *high_priority_thread(void *arg)
{
    msg_t msg;
    while (1) {
        msg_receive(&msg);
        printf("High Priority Thread Received: %d\n", msg.content.value);
    }
    return NULL;
}

void *low_priority_thread(void *arg)
{
    msg_t msg;
    while (1) {
        msg_receive(&msg);
        printf("Low Priority Thread Received: %d\n", msg.content.value);
    }
    return NULL;
}

int main(void)
{
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    kernel_pid_t pid_high = thread_create(stack_high, sizeof(stack_high),
                                          THREAD_PRIORITY_MAIN - 1, 0,
                                          high_priority_thread, NULL, "high");

    kernel_pid_t pid_low = thread_create(stack_low, sizeof(stack_low),
                                         THREAD_PRIORITY_MAIN - 2, 0,
                                         low_priority_thread, NULL, "low");

    msg_t msg;
    msg.type = 0;
    msg.content.value = 123;

    for (int i = 0; i < 10; i++) {
        msg_send(&msg, pid_high);
        ztimer_sleep(ZTIMER_MSEC, 100);
        msg_send(&msg, pid_low);
        ztimer_sleep(ZTIMER_MSEC, 100);
    }

    return 0;
}
