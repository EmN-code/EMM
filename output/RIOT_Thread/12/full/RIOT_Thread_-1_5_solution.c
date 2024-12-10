#include <msg.h>
#include <ztimer.h>
#include <netinet/in.h>
#include <thread.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdatomic.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <xtimer.h>
#include <event/thread.h>
#include <mbox.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <time_units.h>
#include <kernel_defines.h>
#include <paho_mqtt.h>
#include <MQTTClient.h>
#include <mma8x5x_regs.h>

#define SENDER_PRIO      (THREAD_PRIORITY_MAIN + 1)
#define RECEIVER_PRIO    (THREAD_PRIORITY_MAIN + 2)
#define THREAD_STACKSIZE THREAD_STACKSIZE_DEFAULT
#define NUM_MSGS         (16U)

static char sender_stack[THREAD_STACKSIZE];
static char receiver_stack[THREAD_STACKSIZE];
static msg_t msg_queue[NUM_MSGS];

void *sender(void *arg) {
    (void)arg;
    msg_t msg;

    for (unsigned i = 0; i < 10; i++) {
        msg.type = i;
        while (msg_send(&msg, thread_get(SENDER_PRIO)) != 1) {
            printf("[Sender] Message send failed\n");
            xtimer_sleep(1);
        }
        printf("[Sender] Message %d sent\n", i);
        xtimer_sleep(1);
    }
    return NULL;
}

void *receiver(void *arg) {
    (void)arg;
    msg_t msg;

    for (unsigned i = 0; i < 10; i++) {
        if (msg_receive(&msg) == 1) {
            printf("[Receiver] Message %d received\n", msg.type);
        }
        xtimer_sleep(1);
    }
    return NULL;
}

int main(void) {
    msg_init_queue(msg_queue, NUM_MSGS);

    kernel_pid_t sender_pid = thread_create(sender_stack, sizeof(sender_stack),
                                            SENDER_PRIO, THREAD_CREATE_STACKTEST,
                                            sender, NULL, "sender");

    kernel_pid_t receiver_pid = thread_create(receiver_stack, sizeof(receiver_stack),
                                              RECEIVER_PRIO, THREAD_CREATE_STACKTEST,
                                              receiver, NULL, "receiver");

    while (1) {
        thread_yield();
    }

    return 0;
}