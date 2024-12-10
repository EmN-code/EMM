
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
#include "dht_params.h"
#include "stdlib.h"
#include "mbox.h"
#include "stdio.h"
#include "xtimer.h"
#include "arpa/inet.h"
#include "thread.h"
#include "mma8x5x_regs.h"
#include "unistd.h"
#include "stdatomic.h"
#include "mma8x5x.h"
#include "MQTTClient.h"
#include "mma8x5x_params.h"

#define STACKSIZE 1024
#define PRIORITY_HIGH 5
#define PRIORITY_LOW 10

kernel_pid_t sender_pid;
kernel_pid_t receiver_pid;

void *sender_thread(void *arg) {
    msg_t msg, reply;
    msg.type = 1;
    msg.content.value = 123;

    while (1) {
        msg_send_receive(&msg, &reply, receiver_pid);
        printf("Sender: Sent message and received reply: %d\n", reply.content.value);
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }
    return NULL;
}

void *receiver_thread(void *arg) {
    msg_t msg, reply;

    while (1) {
        msg_receive(&msg);
        printf("Receiver: Received message: %d\n", msg.content.value);
        reply.type = 2;
        reply.content.value = msg.content.value + 1;
        msg_reply(&msg, &reply);
    }
    return NULL;
}

int main(void) {
    char stack_sender[STACKSIZE];
    char stack_receiver[STACKSIZE];

    sender_pid = thread_create(stack_sender, sizeof(stack_sender), PRIORITY_HIGH, 0, sender_thread, NULL, "sender");
    receiver_pid = thread_create(stack_receiver, sizeof(stack_receiver), PRIORITY_LOW, 0, receiver_thread, NULL, "receiver");

    return 0;
}
