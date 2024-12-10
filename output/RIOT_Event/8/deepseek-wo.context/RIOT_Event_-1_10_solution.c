
#include <stdio.h>
#include "xtimer.h"
#include "evtimer.h"
#include "thread.h"

#define INTERVAL_MS (1000000U)  // 1 second in microseconds

static evtimer_t evtimer;
static evtimer_msg_t evtimer_msg;

void event_callback(void *arg) {
    (void)arg;
    printf("Event triggered!\n");
}

int main(void) {
    // Initialize the event timer
    evtimer_init(&evtimer);

    // Set up the event message
    evtimer_msg.event.offset = INTERVAL_MS;
    evtimer_msg.event.callback = event_callback;
    evtimer_msg.event.arg = NULL;

    // Add the event to the timer
    evtimer_add_msg(&evtimer, &evtimer_msg);

    // Main loop to keep the thread alive
    while (1) {
        xtimer_usleep(INTERVAL_MS);
    }

    return 0;
}
