#include <xtimer.h>
#include <ztimer.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <arpa/inet.h>
#include <string.h>
#include <netinet/in.h>
#include "kernel_defines.h"
#include "event/thread.h"
#include "xtimer.h"
#include "msg.h"
#include "thread.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "dht_params.h"
#include "mma8x5x_params.h"
#include "mma8x5x_regs.h"
#include "mma8x5x.h"
#include "paho_mqtt.h"
#include "MQTTClient.h"
#include "timex.h"
#include "mbox.h"
#include "dht.h"

#define MAIN_QUEUE_SIZE 8
#define MSG_OK 0

enum {
    THREAD_PRIORITY_MAIN = 10,
    THREAD_PRIORITY_SENDER = 8,
    THREAD_PRIORITY_RECEIVER = 9
};

char thread_stack_sender[THREAD_STACKSIZE_MAIN];
char thread_stack_receiver[THREAD_STACKSIZE_MAIN];

msg_t msg_queue[MAIN_QUEUE_SIZE];

void *sender_thread(void *arg) {
    (void)arg;
    msg_t msg;
    msg.type = 0x1234;

    while (1) {
        printf("Sender: Sending message\n");
        msg.content.value = 42;
        msg_send(&msg, thread_getpid());
        ztimer_sleep(ZTIMER_MSEC, 3000);
    }
    return NULL;
}

void *receiver_thread(void *arg) {
    (void)arg;
    msg_t msg;

    while (1) {
        if (msg_receive(&msg) == MSG_OK) {
            printf("Receiver: Received message with value: %lu\n", msg.content.value);
        }
    }
    return NULL;
}

int main(void) {
    msg_init_queue(msg_queue, MAIN_QUEUE_SIZE);

    kernel_pid_t thread_sender_pid = thread_create(thread_stack_sender,
                                                   THREAD_STACKSIZE_MAIN,
                                                   THREAD_PRIORITY_SENDER,
                                                   CREATE_STACKTEST,
                                                   sender_thread,
                                                   NULL,
                                                   "sender_thread");

    kernel_pid_t thread_receiver_pid = thread_create(thread_stack_receiver,
                                                     THREAD_STACKSIZE_MAIN,
                                                     THREAD_PRIORITY_RECEIVER,
                                                     CREATE_STACKTEST,
                                                     receiver_thread,
                                                     NULL,
                                                     "receiver_thread");

    (void)thread_sender_pid; // To avoid compiler warning if unused
    (void)thread_receiver_pid; // To avoid compiler warning if unused

    return 0;
}