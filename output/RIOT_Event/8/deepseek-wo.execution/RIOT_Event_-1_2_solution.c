
#include <stdio.h>
#include "xtimer.h"
#include "timex.h"
#include "event/thread.h"
#include "time_units.h"
#include "evtimer_mbox.h"
#include "ztimer.h"
#include "evtimer_msg.h"

#define PERIODIC_INTERVAL (1 * US_PER_SEC)  // 1 second interval

static evtimer_t periodic_timer;
static evtimer_event_t periodic_event;

void periodic_callback(void *arg) {
    (void)arg;
    printf("Periodic callback triggered!\n");
}

int main(void) {
    // Initialize the event timer
    evtimer_init(&periodic_timer);

    // Set up the periodic event
    periodic_event.event.callback = periodic_callback;
    periodic_event.event.arg = NULL;

    // Add the periodic event to the timer
    evtimer_add_periodic(&periodic_timer, &periodic_event, PERIODIC_INTERVAL);

    // Main loop to keep the program running
    while (1) {
        xtimer_sleep(1);  // Sleep for 1 second to keep the main thread alive
    }

    return 0;
}
