#include <xtimer.h>
#include <unistd.h>
#include <timex.h>
#include <time_units.h>
#include <stdbilitc.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdatomic.h>
#include <evtimer_mbox.h>
#include <evtimer_msg.h>
#include <evtimer.h>
#include <msg.h>
#include <mbox.h>
#include <kernel_defines.h>
#include <ztimer.h>

#define QUEUE_SIZE (8)
#define TIMER_INTERVAL (1 * US_PER_SEC)

// Message types
#define MY_EVENT 0x1234

static mbox_t mbox;
static msg_t queue[QUEUE_SIZE];
static evtimer_t evtimer;
static evtimer_msg_event_t event;

void *sender_thread(void *arg) {
    (void)arg;
    msg_t msg;
    msg.type = MY_EVENT;

    while (1) {
        msg_send(&msg, mbox);
        xtimer_usleep(TIMER_INTERVAL);
    }
    return NULL;
}

void *receiver_thread(void *arg) {
    (void)arg;
    msg_t msg;

    while (1) {
        msg_receive(&msg);
        if (msg.type == MY_EVENT) {
            puts("Event received!");
        }
    }
    return NULL;
}

int main(void) {
    // Initialize mailbox
    mbox_init(&mbox, queue, QUEUE_SIZE);
    
    // Initialize event timer
    evtimer_init_msg(&evtimer, mbox, MY_EVENT);
    evtimer_add_msg(&evtimer, &event, TIMER_INTERVAL, MY_EVENT);

    // Create threads for sending and receiving
    thread_create(sender_thread, NULL, NULL);
    thread_create(receiver_thread, NULL, NULL);

    return 0;
}