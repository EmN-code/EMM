#include <xtimer.h>  // Include required RIOT headers for timer
#include <stdio.h>
#include <evtimer.h>
#include <msg.h>      // Include required RIOT headers for messaging

#define INTERVAL (1000000U)  // Timer interval set to 1 second (1,000,000 microseconds)

static void _evtimer_mbox_handler(evtimer_msg_event_t *event)
{
    printf("Event triggered with message ID: %d\n", event->msg.content.value);
}

int main(void) {
    // Define message queue
    msg_t queue[8];

    // Initialize the message queue
    msg_init_queue(queue, 8);

    // Define the evtimer and event
    evtimer_t evtimer;
    evtimer_msg_event_t event;

    // Initialize the evtimer
    evtimer_init_mbox(&evtimer);

    // Fill the event structure
    event.mbox = queue;
    event.event = EVTIMER_ONCE;
    event.msg.content.value = 42; // Example payload

    // Add the event to the evtimer with the specified interval
    evtimer_add_mbox(&evtimer, &event, INTERVAL);

    printf("Event timer set with interval %u microseconds.\n", INTERVAL);

    // Run the message loop
    msg_t msg;
    while (1) {
        if (msg_receive(&msg) > 0) {
            _evtimer_mbox_handler((evtimer_msg_event_t *)&msg);
        }
    }

    return 0;
}