
#include "net/gcoap.h"
#include "paho_mqtt.h"
#include "ztimer.h"
#include "stddef.h"
#include "stdbool.h"
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
#include "mbox.h"
#include "stdio.h"
#include "xtimer.h"
#include "arpa/inet.h"
#include "thread.h"
#include "net/emcute.h"
#include "mma8x5x_regs.h"
#include "unistd.h"
#include "stdatomic.h"
#include "mma8x5x.h"
#include "MQTTClient.h"
#include "mma8x5x_params.h"

#define STACKSIZE 512
#define PRIORITY_HIGH 7
#define PRIORITY_LOW 5

char stack_high[STACKSIZE];
char stack_low[STACKSIZE];

kernel_pid_t pid_high, pid_low;

void *high_priority_thread(void *arg)
{
    (void)arg;
    msg_t msg, reply;

    while (1) {
        msg_receive(&msg);
        printf("High priority thread received message: %d\n", msg.content.value);

        reply.content.value = msg.content.value + 1;
        msg_reply(&msg, &reply);
    }

    return NULL;
}

void *low_priority_thread(void *arg)
{
    (void)arg;
    msg_t msg, reply;

    while (1) {
        msg_receive(&msg);
        printf("Low priority thread received message: %d\n", msg.content.value);

        reply.content.value = msg.content.value + 1;
        msg_reply(&msg, &reply);
    }

    return NULL;
}

int main(void)
{
    msg_t msg, reply;

    pid_high = thread_create(stack_high, sizeof(stack_high), PRIORITY_HIGH, 0, high_priority_thread, NULL, "high_priority");
    pid_low = thread_create(stack_low, sizeof(stack_low), PRIORITY_LOW, 0, low_priority_thread, NULL, "low_priority");

    msg.content.value = 1;
    msg_send_receive(&msg, &reply, pid_high);
    printf("Main thread received reply from high priority thread: %d\n", reply.content.value);

    msg.content.value = 10;
    msg_send_receive(&msg, &reply, pid_low);
    printf("Main thread received reply from low priority thread: %d\n", reply.content.value);

    return 0;
}
