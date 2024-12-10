
#include "paho_mqtt.h"
#include "ztimer.h"
#include "stddef.h"
#include "stdbool.h"
#include "msg.h"
#include "evtimer_mbox.h"
#include "timex.h"
#include "event/thread.h"
#include "evtimer_msg.h"
#include "time_units.h"
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
#define PRIORITY_HIGH 7
#define PRIORITY_LOW 3

static char stack_high[STACKSIZE];
static char stack_low[STACKSIZE];

static void *high_priority_thread(void *arg)
{
    msg_t msg, reply;
    msg.type = 1;
    msg.content.value = 123;

    while (1) {
        msg_send_receive(&msg, &reply, thread_getpid());
        printf("High priority thread received reply: %d\n", reply.content.value);
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }

    return NULL;
}

static void *low_priority_thread(void *arg)
{
    msg_t msg, reply;

    while (1) {
        msg_receive(&msg);
        printf("Low priority thread received message: %d\n", msg.content.value);
        reply.type = 2;
        reply.content.value = 456;
        msg_reply(&msg, &reply);
    }

    return NULL;
}

int main(void)
{
    kernel_pid_t pid_high, pid_low;

    pid_high = thread_create(stack_high, sizeof(stack_high), PRIORITY_HIGH, 0, high_priority_thread, NULL, "high_priority");
    pid_low = thread_create(stack_low, sizeof(stack_low), PRIORITY_LOW, 0, low_priority_thread, NULL, "low_priority");

    if (pid_high <= KERNEL_PID_UNDEF || pid_low <= KERNEL_PID_UNDEF) {
        printf("Thread creation failed\n");
        return 1;
    }

    printf("Threads created successfully\n");

    return 0;
}
