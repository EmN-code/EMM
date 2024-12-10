#include <ztimer.h>
#include <stdatomic.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <xtimer.h>
#include "thread.h"
#include "msg.h"
#include "netinet/in.h"
#include "arpa/inet.h"
#include "paho_mqtt.h"
#include "MQTTClient.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "dht_params.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"
#include "kernel_defines.h"
#include "event/thread.h"

#define SENDER_THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 1)
#define RECEIVER_THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 2)
#define STACKSIZE (THREAD_STACKSIZE_MAIN)

static char sender_stack[STACKSIZE];
static char receiver_stack[STACKSIZE];

msg_t queue[8];

void *sender_thread(void *arg) {
    (void)arg;
    kernel_pid_t receiver_pid = *((kernel_pid_t *)arg);
    msg_t msg;
    msg.content.value = 42;

    while (true) {
        msg_send(&msg, receiver_pid);
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }

    return NULL;
}

void *receiver_thread(void *arg) {
    (void)arg;
    msg_t msg;

    msg_init_queue(queue, ARRAY_SIZE(queue));

    while (true) {
        msg_receive(&msg);
        printf("Received message with value: %" PRIu32 "\n", msg.content.value);

        // Here we could call another function or perform a task
    }

    return NULL;
}

int main(void) {
    kernel_pid_t receiver_pid = thread_create(receiver_stack, STACKSIZE, RECEIVER_THREAD_PRIORITY, 0,
                                               receiver_thread, NULL, "receiver");

    if (receiver_pid <= KERNEL_PID_UNDEF) {
        puts("Error creating receiver thread");
        return 1;
    }

    kernel_pid_t sender_pid = thread_create(sender_stack, STACKSIZE, SENDER_THREAD_PRIORITY, 0,
                                             sender_thread, &receiver_pid, "sender");

    if (sender_pid <= KERNEL_PID_UNDEF) {
        puts("Error creating sender thread");
        return 1;
    }

    return 0;
}