
#include <xtimer.h>
#include <thread.h>
#include <mbox.h>
#include <string.h>
#include <stddef.h>
#include <stdatomic.h>
#include <paho_mqtt.h>
#include <evtimer_msg.h>
#include <mma8x5x.h>
#include <time_units.h>
#include <periph/gpio.h>
#include <event/thread.h>
#include <unistd.h>
#include <stdlib.h>
#include <evtimer_mbox.h>
#include <ztimer.h>
#include <msg.h>
#include <timex.h>
#include <mma8x5x_regs.h>
#include <stdbool.h>
#include <event/periodic_callback.h>
#include <periph/rtc.h>
#include <stdio.h>
#include <periph/i2c.h>
#include <MQTTClient.h>

#define STACKSIZE 512
#define NUM_MESSAGES 8

static char stack_high[STACKSIZE];
static char stack_low[STACKSIZE];

static msg_t msg_queue[NUM_MESSAGES];

void *high_priority_thread(void *arg) {
    (void)arg;
    msg_t msg;

    while (1) {
        msg_receive(&msg);
        printf("High priority thread received message: %d\n", msg.content.value);
    }

    return NULL;
}

void *low_priority_thread(void *arg) {
    (void)arg;
    msg_t msg;

    while (1) {
        msg_receive(&msg);
        printf("Low priority thread received message: %d\n", msg.content.value);
    }

    return NULL;
}

int main(void) {
    kernel_pid_t pid_high, pid_low;
    msg_t msg;

    msg_init_queue(msg_queue, NUM_MESSAGES);

    pid_high = thread_create(stack_high, sizeof(stack_high), THREAD_PRIORITY_MAIN + 1, 0, high_priority_thread, NULL, "high_priority");
    pid_low = thread_create(stack_low, sizeof(stack_low), THREAD_PRIORITY_MAIN - 1, 0, low_priority_thread, NULL, "low_priority");

    msg.type = 0;
    msg.content.value = 123;

    while (1) {
        msg_send(&msg, pid_high);
        xtimer_sleep(1);
        msg_send(&msg, pid_low);
        xtimer_sleep(1);
    }

    return 0;
}
