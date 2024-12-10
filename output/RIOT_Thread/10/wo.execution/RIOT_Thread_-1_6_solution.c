#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdatomic.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdio.h>
#include "event/thread.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"
#include "kernel_defines.h"
#include "mbox.h"
#include "msg.h"
#include "net/emcute.h"
#include "net/gcoap.h"
#include "net/sock/udp.h"
#include "net/sock/util.h"
#include "paho_mqtt.h"
#include "mma8x5x.h"
#include "mma8x5x_params.h"
#include "mma8x5x_regs.h"
#include "MQTTClient.h"
#include "thread.h"
#include "timex.h"
#include "time_units.h"
#include "xtimer.h"
#include "ztimer.h"
#include "dht_params.h"

#define SENDER_THREAD_PRIORITY      (THREAD_PRIORITY_MAIN - 1)
#define RECEIVER_THREAD_PRIORITY    (THREAD_PRIORITY_MAIN - 2)

#define STACKSIZE                   (THREAD_STACKSIZE_MAIN)

static char sender_thread_stack[STACKSIZE];
static char receiver_thread_stack[STACKSIZE];

static kernel_pid_t sender_pid;
static kernel_pid_t receiver_pid;

void *sender_thread(void *arg) {
    (void)arg;
    msg_t msg;
    msg_t reply;
    msg.content.value = 1234; // Example message value

    while (1) {
        msg_send_receive(&msg, &reply, receiver_pid);
        printf("Sender: sent message with value %d and received reply with value %d\n", msg.content.value, reply.content.value);
        xtimer_sleep(1);
    }

    return NULL;
}

void *receiver_thread(void *arg) {
    (void)arg;
    msg_t msg;
    msg_t reply;

    while (1) {
        msg_receive(&msg);
        printf("Receiver: received message with value %d\n", msg.content.value);
        reply.content.value = msg.content.value + 1;
        msg_reply(&msg, &reply);
    }

    return NULL;
}

int main(void) {
    printf("Starting message passing example\n");

    msg_init_queue(sender_thread_stack, STACKSIZE);
    msg_init_queue(receiver_thread_stack, STACKSIZE);

    receiver_pid = thread_create(receiver_thread_stack, sizeof(receiver_thread_stack), RECEIVER_THREAD_PRIORITY, THREAD_CREATE_STACKTEST, receiver_thread, NULL, "receiver_thread");
    sender_pid = thread_create(sender_thread_stack, sizeof(sender_thread_stack), SENDER_THREAD_PRIORITY, THREAD_CREATE_STACKTEST, sender_thread, NULL, "sender_thread");

    while (1) {
        // Main thread loop
        xtimer_sleep(5);
    }

    return 0;
}