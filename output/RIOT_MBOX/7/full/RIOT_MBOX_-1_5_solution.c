#include <xtimer.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>
#include <kernel_defines.h>
#include <event/thread.h>
#include <mbox.h>
#include <evtimer_mbox.h>
#include <evtimer_msg.h>
#include <dht_params.h>
#include <mma8x5x.h>
#include <mma8x5x_regs.h>
#include <mma8x5x_params.h>
#include <paho_mqtt.h>
#include <time_units.h>
#include "MQTTClient.h"

#define MAIN_QUEUE_SIZE (8)
static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

static mbox_t mbox;
static msg_t mbox_queue[8];
static evtimer_t evtimer;
static evtimer_mbox_event_t evtimer_event;

void *event_thread(void *arg) {
    (void) arg;
    msg_t msg;
    while (1) {
        // wait for a message
        if (msg_receive(&msg) == 1) {
            printf("Received message\n");
        }
    }
    return NULL;
}

int main(void) {
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);

    // Initialize mailbox
    mbox_init(&mbox, mbox_queue, 8);

    // Initialize evtimer for mbox events
    evtimer_init_mbox(&evtimer);
    evtimer_event.event.interval = 1000000; // 1 second interval
    evtimer_add_mbox(&evtimer, &evtimer_event, &mbox);

    // Create a new thread for handling events
    char stack[THREAD_STACKSIZE_MAIN];
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST, event_thread, NULL, "event_thread");
    
    return 0;
}