#include <xtimer.h>
#include <ztimer.h>
#include <timex.h>
#include <unistd.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdatomic.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <mbox.h>
#include <msg.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <kernel_defines.h>
#include <time_units.h>

// Define a message queue size
#define QUEUE_SIZE 8

// Event handler function
void periodic_event_handler(void *arg) {
    puts("Periodic event triggered.");
}

int main(void) {
    // Define a message queue for the mbox
    msg_t msg_queue[QUEUE_SIZE];
    msg_init_queue(msg_queue, QUEUE_SIZE);

    // Define and initialize mbox
    mbox_t mbox;
    mbox_init(&mbox, msg_queue, QUEUE_SIZE);

    // Initialize an evtimer
    evtimer_t evtimer;
    evtimer_mbox_init(&evtimer, &mbox);

    // Create an evtimer event
    evtimer_event_t timer_event = {
        .event.handler = periodic_event_handler,
        .event.arg = NULL
    };

    // Add the event to the evtimer with a periodic interval of 1 second
    evtimer_add_periodic(&evtimer, &timer_event, 1000000);

    // Main loop
    while (1) {
        // Wait for messages on the mbox, then handle appropriately
        msg_t msg;
        mbox_receive(&mbox, &msg);
        evtimer_mbox_handle(&evtimer, &msg);
    }

    return 0;
}