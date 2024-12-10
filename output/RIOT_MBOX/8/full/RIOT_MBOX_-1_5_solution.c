#include <xtimer.h>
#include <stdbool.h>
#include <stdatomic.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <unistd.h>

#include "event/thread.h"
#include "ztimer.h"
#include "msg.h"
#include "thread.h"
#include "mbox.h"

#include "evtimer_mbox.h"
#include "evtimer_msg.h"

#include "mqttclient.h"
#include "paho_mqtt.h"

#include "mma8x5x_regs.h"
#include "dht_params.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "dht.h"

#include "kernel_defines.h"
#include "timex.h"
#include "time_units.h"

#define PRIORITY_MAIN    (THREAD_PRIORITY_MAIN - 1)
#define MBOX_SIZE        (8)

static msg_t queue[MBOX_SIZE];
static kernel_pid_t msg_thread_pid;

void *msg_thread(void *arg) {
    msg_init_queue(queue, MBOX_SIZE);
    msg_t msg;
    (void)arg;

    while (1) {
        msg_receive(&msg);
        printf("Received message, type: %u\n", msg.type);
    }

    return NULL;
}

int main(void) {
    char stack[THREAD_STACKSIZE_MAIN];

    msg_thread_pid = thread_create(stack, sizeof(stack), PRIORITY_MAIN, THREAD_CREATE_STACKTEST, msg_thread, NULL, "msg_thread");

    msg_t msg;
    msg.type = 0x1234;

    while (1) {
        msg_send(&msg, msg_thread_pid);
        printf("Sent message\n");
        xtimer_sleep(3);
    }

    return 0;
}
