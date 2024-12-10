
#include "xtimer.h"
#include "evtimer.h"
#include "msg.h"

#define TIMER_INTERVAL (1 * 1000 * 1000U)  // 1 second in microseconds

static evtimer_t evtimer;
static msg_t msg_queue[8];

void event_periodic_callback(void *arg) {
    (void)arg;
    printf("Event periodic callback triggered!\n");
}

int main(void) {
    // Initialize the message queue
    msg_init_queue(msg_queue, sizeof(msg_queue) / sizeof(msg_queue[0]));

    // Initialize the event timer
    evtimer_init(&evtimer);

    // Create an event for the periodic callback
    evtimer_event_t event;
    event.callback = event_periodic_callback;
    event.arg = NULL;

    // Add the event to the event timer with a 1-second interval
    evtimer_add(&evtimer, &event, TIMER_INTERVAL);

    // Main loop to keep the application running
    while (1) {
        msg_t msg;
        msg_receive(&msg);
    }

    return 0;
}
