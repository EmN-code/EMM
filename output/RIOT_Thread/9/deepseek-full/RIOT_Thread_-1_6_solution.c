
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdatomic.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <kernel_defines.h>
#include <time_units.h>
#include <timex.h>
#include <xtimer.h>
#include <ztimer.h>
#include <event/thread.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <mbox.h>
#include <dht.h>
#include <dht_params.h>
#include <mma8x5x_regs.h>
#include <mma8x5x.h>
#include <mma8x5x_params.h>
#include <MQTTClient.h>
#include <paho_mqtt.h>

#define STACKSIZE 512
#define NUM_MESSAGES 8

static char stack_high[STACKSIZE];
static char stack_low[STACKSIZE];

static msg_t msg_queue[NUM_MESSAGES];

void *thread_high_priority(void *arg) {
    (void)arg;
    msg_t msg;
    while (1) {
        msg_receive(&msg);
        printf("High priority thread received message: %d\n", msg.content.value);
    }
    return NULL;
}

void *thread_low_priority(void *arg) {
    (void)arg;
    msg_t msg;
    while (1) {
        msg_receive(&msg);
        printf("Low priority thread received message: %d\n", msg.content.value);
    }
    return NULL;
}

int main(void) {
    msg_init_queue(msg_queue, NUM_MESSAGES);

    kernel_pid_t pid_high = thread_create(stack_high, sizeof(stack_high), THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST, thread_high_priority, NULL, "high_priority");
    kernel_pid_t pid_low = thread_create(stack_low, sizeof(stack_low), THREAD_PRIORITY_MAIN - 2, THREAD_CREATE_STACKTEST, thread_low_priority, NULL, "low_priority");

    msg_t msg;
    msg.type = 0;
    msg.content.value = 123;

    while (1) {
        msg_send(&msg, pid_high);
        msg_send(&msg, pid_low);
        xtimer_sleep(1);
    }

    return 0;
}
