#include <ztimer.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdatomic.h>
#include <thread.h>
#include <timex.h>
#include <time_units.h>
#include <stdlib.h>
#include <msg.h>
#include <mbox.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <kernel_defines.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <paho_mqtt.h>
#include <MQTTClient.h>
#include <event/thread.h>

#define SENDER_PRIO (THREAD_PRIORITY_MAIN - 1)
#define RECEIVER_PRIO (THREAD_PRIORITY_MAIN - 2)
#define STACKSIZE (THREAD_STACKSIZE_MAIN)

static msg_t msg_queue[8];
static mbox_t my_mbox;

void *sender_thread(void *arg) {
    (void)arg;
    msg_t msg;
    msg.type = 1;
    while (1) {
        printf("Sending message\n");
        mbox_put(&my_mbox, &msg);
        ztimer_sleep(ZTIMER_USEC, 1000000);
    }
    return NULL;
}

void *receiver_thread(void *arg) {
    (void)arg;
    msg_t msg;
    while (msg_receive(&msg)) {
        if (msg.type == 1) {
            printf("Received message\n");
        }
    }
    return NULL;
}

int main(void) {
    mbox_init(&my_mbox, msg_queue, 8);

    char sender_stack[STACKSIZE];
    char receiver_stack[STACKSIZE];

    thread_create(sender_stack, sizeof(sender_stack), SENDER_PRIO,
                  THREAD_CREATE_STACKTEST, sender_thread, NULL, "sender");

    thread_create(receiver_stack, sizeof(receiver_stack), RECEIVER_PRIO,
                  THREAD_CREATE_STACKTEST, receiver_thread, NULL, "receiver");

    while (1) {
        thread_yield();
    }

    return 0;
}