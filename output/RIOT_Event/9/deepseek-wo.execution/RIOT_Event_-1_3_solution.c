
#include <stdio.h>
#include <unistd.h>
#include <stddef.h>
#include "msg.h"
#include "mbox.h"
#include "evtimer_mbox.h"

#define QUEUE_SIZE (16)

static msg_t msg_queue[QUEUE_SIZE];
static mbox_t mbox;
static evtimer_t evtimer;

static void periodic_callback(void *arg) {
    printf("Periodic callback triggered!\n");
}

int main(void) {
    // Initialize the message queue and mailbox
    mbox_init(&mbox, msg_queue, QUEUE_SIZE);

    // Initialize the event timer for mailbox events
    evtimer_init_mbox(&evtimer);

    // Create an event for the periodic callback
    evtimer_mbox_event_t event;
    event.event.callback = (evtimer_callback_t)periodic_callback;
    event.event.arg = NULL;

    // Add the event to the event timer with a 1-second interval
    evtimer_add_mbox(&evtimer, &event, &mbox);

    // Main loop to receive messages
    msg_t msg;
    while (1) {
        msg_receive(&msg);
        // Handle the received message if needed
    }

    return 0;
}
