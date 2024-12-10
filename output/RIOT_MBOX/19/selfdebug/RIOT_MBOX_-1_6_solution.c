#include "evtimer.h"
#include "msg.h"
#include "kernel.h"
#include <stdio.h>

#define INTERVAL (1000000U) // 1 second interval
#define QUEUE_SIZE (8) // Size of the message queue

static msg_t queue[QUEUE_SIZE];
static evtimer_t evtimer;
static evtimer_event_t event;
static mbox_t mbox;

void timer_handler(void *arg) {
    (void)arg;
    printf("Event triggered\n");
}

int main(void) {
    msg_init_queue(queue, QUEUE_SIZE);

    // Initialize the event timer with the message handler
    evtimer_init(&evtimer, timer_handler);

    // Initialize mbox
    mbox_init(&mbox, queue, QUEUE_SIZE);

    // Configure the event
    evtimer_set_msg(&event, INTERVAL, &mbox, NULL);

    // Add the event to the timer
    evtimer_add(&evtimer, &event);

    // Start the message loop
    while (1) {
        msg_t msg;
        if (mbox_try_get(&mbox, &msg) == 1) {
            printf("Received message\n");
        }
    }

    return 0;
}