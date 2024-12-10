
#include "xtimer.h"
#include "evtimer.h"
#include "msg.h"

#define PERIODIC_INTERVAL (1 * 1000 * 1000u)  // 1 second in microseconds

static evtimer_t evtimer;
static evtimer_event_t evtimer_event;

void periodic_callback(void *arg) {
    (void)arg;
    printf("Periodic callback triggered!\n");
}

int main(void) {
    // Initialize the event timer
    evtimer_init(&evtimer);

    // Initialize the event
    evtimer_event_init_msg(&evtimer_event, xtimer_ticks32(PERIODIC_INTERVAL), periodic_callback, NULL);

    // Add the event to the event timer
    evtimer_add_event(&evtimer, &evtimer_event);

    // Main loop to keep the application running
    while (1) {
        msg_t msg;
        msg_receive(&msg);
    }

    return 0;
}
