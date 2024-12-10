#include "ztimer.h"
#include "msg.h"
#include "evtimer.h"

#define INTERVAL (1000000U) // 1 second intervals in microsecond

// Global message box and event timer
static msg_t msg_queue[8];
static evtimer_t evtimer;

// Event handler function for handling timer events
void on_timer_event(evtimer_event_t *event) {
    // Example action: print a message
    puts("Timer event received!");
}

int main(void) {
    // Initialize message queue
    msg_init_queue(msg_queue, 8);

    // Initialize event timer
    evtimer_init(&evtimer, ZTIMER_MSEC);

    // Setup event
    evtimer_event_t timer_event;
    timer_event.callback = on_timer_event;
    timer_event.period = INTERVAL;

    // Add the event to the evtimer
    evtimer_add(&evtimer, &timer_event, INTERVAL);

    // Main event loop
    while (1) {
        // Wait for and handle events
        msg_t m;
        msg_receive(&m);
    }
    return 0;
}