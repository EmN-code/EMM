
#include <stdio.h>  // Include stdio.h for puts function
#include "event/periodic_callback.h"  // Include the correct header for event_periodic_callback_t
#include "ztimer.h"  // Include ztimer for timing functionality
#include "msg.h"  // Include msg.h for message queue functionality

#define INTERVAL (1000000U)  // Define the interval in microseconds

static msg_t msg_queue[16];  // Define a message queue with 16 slots
static mbox_t mbox;  // Define a message box

// Callback function for the periodic event
void periodic_callback(void *arg) {
    msg_t msg;
    msg.content.value = 42;  // Example message content
    mbox_put(&mbox, &msg);  // Put the message into the message box
}

int main(void) {
    // Initialize the message queue
    msg_init_queue(msg_queue, sizeof(msg_queue) / sizeof(msg_queue[0]));

    // Initialize the message box
    mbox_init(&mbox, msg_queue, sizeof(msg_queue) / sizeof(msg_queue[0]));

    // Create and start the periodic event
    event_periodic_callback_t periodic_event;
    event_periodic_callback_create(&periodic_event, ZTIMER_USEC, INTERVAL, NULL, periodic_callback);
    event_periodic_callback_start(&periodic_event);

    // Main loop to handle messages from the message box
    while (1) {
        msg_t msg;
        mbox_get(&mbox, &msg, 1);  // Blocking get from the message box
        puts("Message received from message box");
    }

    return 0;
}
