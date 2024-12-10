
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdatomic.h>
#include <stddef.h>
#include <mbox.h>
#include <thread.h>
#include <time_units.h>
#include <periph/gpio.h>
#include <event/thread.h>
#include <evtimer_msg.h>
#include <mma8x5x.h>
#include <evtimer_mbox.h>
#include <ztimer.h>
#include <timex.h>
#include <mma8x5x_regs.h>
#include <event/periodic_callback.h>
#include <periph/rtc.h>
#include <periph/i2c.h>
#include <MQTTClient.h>

#define STACKSIZE 1024
#define NUM_MESSAGES 8

static char stack_low[STACKSIZE];
static char stack_high[STACKSIZE];
static msg_t msg_array[NUM_MESSAGES];

void *thread_low_priority(void *arg) {
    (void)arg;
    msg_t msg;
    while (1) {
        msg_receive(&msg);
        printf("Low priority thread received message: %d\n", msg.content.value);
    }
    return NULL;
}

void *thread_high_priority(void *arg) {
    (void)arg;
    msg_t msg;
    while (1) {
        msg_receive(&msg);
        printf("High priority thread received message: %d\n", msg.content.value);
    }
    return NULL;
}

int main(void) {
    msg_init_queue(msg_array, NUM_MESSAGES);

    kernel_pid_t pid_low = thread_create(stack_low, sizeof(stack_low),
                                         THREAD_PRIORITY_MAIN - 1, 0,
                                         thread_low_priority, NULL, "low");

    kernel_pid_t pid_high = thread_create(stack_high, sizeof(stack_high),
                                          THREAD_PRIORITY_MAIN - 2, 0,
                                          thread_high_priority, NULL, "high");

    msg_t msg;
    msg.content.value = 1;

    // Send a message to the low priority thread
    msg_send(&msg, pid_low);

    // Send a message to the high priority thread
    msg.content.value = 2;
    msg_send(&msg, pid_high);

    return 0;
}
