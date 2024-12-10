
#include "xtimer.h"
#include "stdbool.h"
#include "stdatomic.h"
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

#define STACKSIZE (THREAD_STACKSIZE_MAIN)
#define PRIORITY (THREAD_PRIORITY_MAIN - 1)
#define INTERVAL (1U * US_PER_SEC)

static char thread_stack[STACKSIZE];
static msg_t msg_queue[8];

void *thread_handler(void *arg) {
    (void)arg;
    msg_t msg;
    msg.type = 0x1234;

    while (true) {
        msg.content.value = 1;
        msg_send(&msg, thread_getpid());
        xtimer_usleep(INTERVAL);
    }

    return NULL;
}

int main(void) {
    msg_init_queue(msg_queue, 8);
    thread_create(thread_stack, sizeof(thread_stack), PRIORITY, THREAD_CREATE_STACKTEST, thread_handler, NULL, "thread");

    while (1) {
        msg_t msg;
        msg_receive(&msg);
        printf("Received message with type: %04x\n", msg.type);
    }

    return 0;
}