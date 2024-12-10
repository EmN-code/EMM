
#include "net/gcoap.h"
#include "paho_mqtt.h"
#include "ztimer.h"
#include "stddef.h"
#include "stdbool.h"
#include "mutex.h"
#include "msg.h"
#include "dht.h"
#include "evtimer_mbox.h"
#include "timex.h"
#include "kernel_defines.h"
#include "netinet/in.h"
#include "event/thread.h"
#include "evtimer_msg.h"
#include "time_units.h"
#include "net/sock/util.h"
#include "dht_params.h"
#include "stdlib.h"
#include "net/sock/udp.h"
#include "sys/socket.h"
#include "mbox.h"
#include "stdio.h"
#include "xtimer.h"
#include "arpa/inet.h"
#include "thread.h"
#include "string.h"
#include "net/emcute.h"
#include "mma8x5x_regs.h"
#include "unistd.h"
#include "stdatomic.h"
#include "mma8x5x.h"
#include "MQTTClient.h"
#include "mma8x5x_params.h"

#define STACKSIZE 1024
#define NUM_MESSAGES 8

static char stack_high[STACKSIZE];
static char stack_low[STACKSIZE];

static msg_t msg_queue[NUM_MESSAGES];

void *high_priority_thread(void *arg)
{
    (void)arg;
    msg_t msg;
    msg_t reply;

    while (1) {
        msg_receive(&msg);
        printf("High priority thread received message: %lu\n", msg.content.value);

        reply.content.value = msg.content.value + 1;
        msg_reply(&msg, &reply);
    }

    return NULL;
}

void *low_priority_thread(void *arg)
{
    (void)arg;
    msg_t msg;
    msg_t reply;

    while (1) {
        msg_receive(&msg);
        printf("Low priority thread received message: %lu\n", msg.content.value);

        reply.content.value = msg.content.value + 1;
        msg_reply(&msg, &reply);
    }

    return NULL;
}

int main(void)
{
    msg_init_queue(msg_queue, NUM_MESSAGES);

    kernel_pid_t high_pid = thread_create(stack_high, sizeof(stack_high),
                                          THREAD_PRIORITY_MAIN - 1, 0,
                                          high_priority_thread, NULL, "high");

    kernel_pid_t low_pid = thread_create(stack_low, sizeof(stack_low),
                                         THREAD_PRIORITY_MAIN - 2, 0,
                                         low_priority_thread, NULL, "low");

    msg_t msg;
    msg_t reply;

    for (uint32_t i = 0; i < 10; i++) {
        msg.content.value = i;

        if (i % 2 == 0) {
            msg_send_receive(&msg, &reply, high_pid);
        } else {
            msg_send_receive(&msg, &reply, low_pid);
        }

        printf("Main thread received reply: %lu\n", reply.content.value);
    }

    return 0;
}
