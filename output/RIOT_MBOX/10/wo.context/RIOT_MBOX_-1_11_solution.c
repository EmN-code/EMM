#include "ztimer.h"
#include "msg.h"
#include <stdio.h>

#define INTERVAL (2 * ZTIMER_MSEC)  // 2 seconds in milliseconds
#define QUEUE_SIZE (8)

static msg_t msg_queue[QUEUE_SIZE];
static evtimer_event_t event;

static void timeout_callback(void *arg) {
    (void)arg; // Avoid unused parameter warning
    printf("Timeout event occurred\n");
}

int main(void) {
    // Initialize the message queue
    msg_init_queue(msg_queue, QUEUE_SIZE);
    
    // Initialize the event timer
    evtimer_t evtimer;
    ztimer_init(ZTIMER_MSEC);
    evtimer_init(&evtimer);

    // Initialize event
    evtimer_event_init(&event, timeout_callback, NULL);

    // Setup the event to trigger after the interval
    evtimer_set(&evtimer, &event, INTERVAL);
    evtimer_add(&evtimer, &event);

    while (1) {
        msg_t msg;
        msg_receive(&msg);  // Block until a message is received
    }

    return 0;
}