
#include <stdlib.h>
#include <msg.h>
#include <mma8x5x_regs.h>
#include <time_units.h>
#include <xtimer.h>
#include <thread.h>
#include <MQTTClient.h>
#include <stdio.h>
#include <ztimer.h>
#include <stdbool.h>
#include <evtimer_mbox.h>
#include <timex.h>
#include <evtimer_msg.h>
#include <unistd.h>
#include <stddef.h>
#include <mbox.h>
#include <event/thread.h>

#define STACK_SIZE 512
#define MSG_QUEUE_SIZE 8

static char stack_high[STACK_SIZE];
static char stack_low[STACK_SIZE];

static msg_t msg_queue_high[MSG_QUEUE_SIZE];
static msg_t msg_queue_low[MSG_QUEUE_SIZE];

static void *high_priority_thread(void *arg)
{
    msg_t msg;
    msg_init_queue(msg_queue_high, MSG_QUEUE_SIZE);

    while (1) {
        msg_receive(&msg);
        printf("High priority thread received message: %d\n", msg.content.value);
    }

    return NULL;
}

static void *low_priority_thread(void *arg)
{
    msg_t msg;
    msg_init_queue(msg_queue_low, MSG_QUEUE_SIZE);

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

    pid_high = thread_create(stack_high, sizeof(stack_high), THREAD_PRIORITY_MAIN + 1,
                             THREAD_CREATE_STACKTEST, high_priority_thread, NULL, "high");

    pid_low = thread_create(stack_low, sizeof(stack_low), THREAD_PRIORITY_MAIN - 1,
                            THREAD_CREATE_STACKTEST, low_priority_thread, NULL, "low");

    msg.content.value = 1;
    msg_send(&msg, pid_high);

    msg.content.value = 2;
    msg_send(&msg, pid_low);

    return 0;
}
