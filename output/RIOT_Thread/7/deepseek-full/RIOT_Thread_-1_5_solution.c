
#include <stddef.h>
#include <stdbool.h>
#include <timex.h>
#include <kernel_defines.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdatomic.h>
#include <mma8x5x_regs.h>
#include <mma8x5x.h>
#include <mma8x5x_params.h>
#include <paho_mqtt.h>
#include <ztimer.h>
#include <time_units.h>
#include <dht_params.h>
#include <mbox.h>
#include <MQTTClient.h>

#define STACKSIZE 1024
#define NUM_MESSAGES 16

static char stack_high[STACKSIZE];
static char stack_low[STACKSIZE];

static msg_t msg_array[NUM_MESSAGES];
static msg_t msg_queue[NUM_MESSAGES];

static void *high_priority_thread(void *arg)
{
    (void)arg;
    msg_t msg;

    while (1) {
        msg_receive(&msg);
        printf("High priority thread received message: %d\n", msg.content.value);
    }

    return NULL;
}

static void *low_priority_thread(void *arg)
{
    (void)arg;
    msg_t msg;

    while (1) {
        msg_receive(&msg);
        printf("Low priority thread received message: %d\n", msg.content.value);
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
    for (int i = 0; i < 10; i++) {
        msg.content.value = i;
        if (i % 2 == 0) {
            msg_send(&msg, pid_high);
        } else {
            msg_send(&msg, pid_low);
        }
        ztimer_sleep(ZTIMER_MSEC, 100);
    }

    return 0;
}
