#include "ztimer.h"
#include "xtimer.h"
#include "unistd.h"
#include "stdlib.h"
#include "stddef.h"
#include "stdatomic.h"
#include "stdbool.h"
#include "stdio.h"
#include "paho_mqtt.h"
#include "msg.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "mbox.h"
#include "kernel_defines.h"
#include "event/thread.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"
#include "dht_params.h"
#include "dht.h"
#include "time_units.h"
#include "timex.h"
#include "thread.h"
#include "MQTTClient.h"

#define THREAD_STACKSIZE_MAIN (THREAD_STACKSIZE_DEFAULT)
#define THREAD_PRIORITY_MAIN (THREAD_PRIORITY_MAIN - 1)
#define THREAD_PRIORITY_SENDER (THREAD_PRIORITY_MAIN - 2)

char stack_thread[THREAD_STACKSIZE_MAIN];

static kernel_pid_t receiver_pid;

void *receiver(void *arg) {
    msg_t msg;
    (void)arg;
    while (1) {
        msg_receive(&msg);
        printf("[Receiver] Received message with type: %u and value: %lu\n", msg.type, msg.content.value);
    }
    return NULL;
}

void *sender(void *arg) {
    msg_t msg;
    (void)arg;

    msg.type = 0x1234;
    msg.content.value = 5678;

    printf("[Sender] Sending message\n");
    msg_send(&msg, receiver_pid);

    return NULL;
}

int main(void) {
    msg_t queue[8];
    msg_init_queue(queue, 8);

    receiver_pid = thread_create(
        stack_thread,
        THREAD_STACKSIZE_MAIN,
        THREAD_PRIORITY_SENDER,
        0,
        receiver,
        NULL,
        "receiver_thread"
    );

    char stack_sender[THREAD_STACKSIZE_MAIN];
    thread_create(
        stack_sender,
        THREAD_STACKSIZE_MAIN,
        THREAD_PRIORITY_MAIN,
        0,
        sender,
        NULL,
        "sender_thread"
    );

    while (1) {
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }

    return 0;
}