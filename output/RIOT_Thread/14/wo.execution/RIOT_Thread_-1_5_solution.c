#include <unistd.h>
#include <stdlib.h>
#include <stdatomic.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include <msg.h>
#include <event/thread.h>
#include <xtimer.h>
#include <ztimer.h>
#include <timex.h>
#include <dht_params.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <kernel_defines.h>
#include <mma8x5x_regs.h>
#include <mma8x5x.h>
#include <mma8x5x_params.h>
#include <mbox.h>
#include <MQTTClient.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <time_units.h>
#include <paho_mqtt.h>
#include <msg.h>
#include <thread.h>
#include <dht.h>

#define SENDER_PRIORITY (THREAD_PRIORITY_MAIN - 1)
#define RECEIVER_PRIORITY (THREAD_PRIORITY_MAIN + 1)
#define STACKSIZE (THREAD_STACKSIZE_MAIN)
#define MSG_QUEUE_SIZE (16)

static char sender_stack[STACKSIZE];
static char receiver_stack[STACKSIZE];

static msg_t rcv_queue[MSG_QUEUE_SIZE];

void *sender(void *arg) {
    (void)arg;
    msg_t msg;

    for (int i = 0; i < 10; i++) {
        msg.type = 1;
        msg.content.value = i;
        printf("Sender: sending message %d\n", i);
        msg_send(&msg, thread_getpid_of(receiver_stack));
        xtimer_sleep(1);
    }
    return NULL;
}

void *receiver(void *arg) {
    (void)arg;
    msg_t msg;

    msg_init_queue(rcv_queue, MSG_QUEUE_SIZE);

    while (1) {
        msg_receive(&msg);
        printf("Receiver: received message with value %lu\n", (unsigned long)msg.content.value);
    }
    return NULL;
}

int main(void) {
    kernel_pid_t sender_pid = thread_create(sender_stack, STACKSIZE, 
                                             SENDER_PRIORITY, 0, 
                                             sender, NULL, "sender");

    kernel_pid_t receiver_pid = thread_create(receiver_stack, STACKSIZE, 
                                              RECEIVER_PRIORITY, 0, 
                                              receiver, NULL, "receiver");

    while (1) {
        xtimer_sleep(1);
    }

    return 0;
}