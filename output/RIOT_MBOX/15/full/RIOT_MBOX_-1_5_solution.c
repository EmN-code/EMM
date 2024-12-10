#include "xtimer.h"
#include "stdatomic.h"
#include "stdbool.h"
#include "netinet/in.h"
#include "event/thread.h"
#include "mbox.h"
#include "evtimer_mbox.h"
#include "stdio.h"
#include "msg.h"
#include "stdlib.h"
#include "mma8x5x_regs.h"
#include "dht_params.h"
#include "mma8x5x_params.h"
#include "stddef.h"
#include "kernel_defines.h"
#include "ztimer.h"
#include "unistd.h"
#include "evtimer_msg.h"
#include "MQTTClient.h"
#include "paho_mqtt.h"
#include "time_units.h"
#include "mma8x5x.h"
#include "thread.h"
#include "arpa/inet.h"
#include "timex.h"

#define STACKSIZE_THREAD 1024
#define PRIORITY_THREAD 10

static char thread_stack[STACKSIZE_THREAD];
static msg_t msg_queue[8];

void *message_thread(void *arg) {
    (void)arg;
    msg_t msg;
    msg_t reply;

    for (;;) {
        // Prepare a dummy message
        msg.content.ptr = NULL;
        msg_send_receive(&msg, &reply, thread_getpid());
        printf("Message received\n");

        // Simulate waiting
        xtimer_sleep(1);
    }

    return NULL;
}

int main(void) {
    msg_init_queue(msg_queue, 8);

    thread_create(thread_stack, sizeof(thread_stack), PRIORITY_THREAD,
                  THREAD_CREATE_STACKTEST, message_thread, NULL, "msg_thread");

    while (1) {
        // Simulate sending event
        msg_t msg;
        msg.content.ptr = NULL;
        msg_send_receive(&msg, NULL, thread_getpid());

        // Wait between sends
        xtimer_sleep(5);
    }

    return 0;
}