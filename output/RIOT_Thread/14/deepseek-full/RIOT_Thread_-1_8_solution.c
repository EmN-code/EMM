
#include <net/gcoap.h>
#include <paho_mqtt.h>
#include <ztimer.h>
#include <stddef.h>
#include <stdbool.h>
#include <msg.h>
#include <dht.h>
#include <evtimer_mbox.h>
#include <timex.h>
#include <kernel_defines.h>
#include <netinet/in.h>
#include <event/thread.h>
#include <evtimer_msg.h>
#include <time_units.h>
#include <net/sock/util.h>
#include <dht_params.h>
#include <stdlib.h>
#include <net/sock/udp.h>
#include <mbox.h>
#include <stdio.h>
#include <xtimer.h>
#include <arpa/inet.h>
#include <thread.h>
#include <net/emcute.h>
#include <mma8x5x_regs.h>
#include <unistd.h>
#include <stdatomic.h>
#include <mma8x5x.h>
#include <MQTTClient.h>
#include <mma8x5x_params.h>

#define STACKSIZE 512
#define NUM_MESSAGES 16

static char stack_high[STACKSIZE];
static char stack_low[STACKSIZE];

static msg_t msg_queue[NUM_MESSAGES];

void *high_priority_thread(void *arg)
{
    msg_t msg;
    while (1) {
        msg_receive(&msg);
        printf("High priority thread received message: %lu\n", (unsigned long)msg.content.value);
    }
    return NULL;
}

void *low_priority_thread(void *arg)
{
    msg_t msg;
    while (1) {
        msg_receive(&msg);
        printf("Low priority thread received message: %lu\n", (unsigned long)msg.content.value);
    }
    return NULL;
}

int main(void)
{
    msg_init_queue(msg_queue, NUM_MESSAGES);

    kernel_pid_t pid_high = thread_create(stack_high, sizeof(stack_high), THREAD_PRIORITY_MAIN - 1, 0, high_priority_thread, NULL, "high_priority");
    kernel_pid_t pid_low = thread_create(stack_low, sizeof(stack_low), THREAD_PRIORITY_MAIN - 2, 0, low_priority_thread, NULL, "low_priority");

    msg_t msg;
    msg.type = 0;

    for (uint32_t i = 0; i < 10; i++) {
        msg.content.value = i;
        if (i % 2 == 0) {
            msg_send(&msg, pid_high);
        } else {
            msg_send(&msg, pid_low);
        }
        xtimer_sleep(1);
    }

    return 0;
}