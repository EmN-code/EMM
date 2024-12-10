
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "msg.h"
#include "thread.h"
#include "xtimer.h"
#include "timex.h"
#include "mbox.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"
#include "ztimer.h"
#include "mma8x5x_regs.h"
#include "time_units.h"
#include "MQTTClient.h"

#define STACKSIZE 512
#define MSG_QUEUE_SIZE 8

static char stack_high[STACKSIZE];
static char stack_low[STACKSIZE];

static msg_t msg_queue[MSG_QUEUE_SIZE];

static void *high_priority_thread(void *arg)
{
    msg_t msg;
    while (1) {
        msg_receive(&msg);
        printf("High priority thread received message: %d\n", msg.content.value);
    }
    return NULL;
}

static void *low_priority_thread(void *arg)
{
    msg_t msg;
    while (1) {
        msg_receive(&msg);
        printf("Low priority thread received message: %d\n", msg.content.value);
    }
    return NULL;
}

int main(void)
{
    kernel_pid_t pid_high, pid_low;
    msg_t msg;

    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    pid_high = thread_create(stack_high, sizeof(stack_high), THREAD_PRIORITY_MAIN - 1,
                             THREAD_CREATE_STACKTEST, high_priority_thread, NULL, "high");

    pid_low = thread_create(stack_low, sizeof(stack_low), THREAD_PRIORITY_MAIN - 2,
                            THREAD_CREATE_STACKTEST, low_priority_thread, NULL, "low");

    msg.type = 0;
    msg.content.value = 123;

    while (1) {
        xtimer_sleep(1);
        msg_send(&msg, pid_high);
        xtimer_sleep(1);
        msg_send(&msg, pid_low);
    }

    return 0;
}
