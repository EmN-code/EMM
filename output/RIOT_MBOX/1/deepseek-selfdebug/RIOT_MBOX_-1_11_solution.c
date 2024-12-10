
#include <stdio.h>
#include "msg.h"
#include "event.h"
#include "event/periodic_callback.h"

#define INTERVAL_MS (1000U)  // Interval for sending events

static msg_t msg_queue[16];  // Message queue with 16 slots
static event_queue_t event_queue;  // Event queue

static void event_handler(void *arg) {
    (void)arg;
    printf("Event triggered!\n");
}

int main(void) {
    // Initialize the message queue
    msg_init_queue(msg_queue, sizeof(msg_queue) / sizeof(msg_queue[0]));

    // Initialize the event queue
    event_queue_init(&event_queue);

    // Initialize the periodic event callback
    event_periodic_callback_t periodic_event;
    event_periodic_callback_init(&periodic_event, ZTIMER_USEC, &event_queue, event_handler, NULL);

    // Set the interval for the periodic event
    event_periodic_callback_set_interval(&periodic_event, INTERVAL_MS * 1000);

    // Start the periodic event
    event_periodic_callback_start(&periodic_event);

    // Initialize the event thread
    event_thread_init(&event_queue, NULL, 0, THREAD_PRIORITY_MAIN - 1);

    // Main loop to keep the program running
    while (1) {
        msg_t msg;
        msg_receive(&msg);
    }

    return 0;
}
