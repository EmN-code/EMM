
#include "xtimer.h"
#include "timex.h"
#include "thread.h"
#include "event.h"
#include "evtimer.h"

#define PERIODIC_INTERVAL (1 * US_PER_SEC)  // 1 second interval

static evtimer_t periodic_timer;
static evtimer_event_t periodic_event;

void periodic_callback(void *arg) {
    (void)arg;
    printf("Periodic event triggered!\n");
}

int main(void) {
    // Initialize the event timer
    evtimer_init(&periodic_timer);

    // Set up the periodic event
    periodic_event.event.handler = periodic_callback;
    periodic_event.event.arg = NULL;

    // Add the periodic event to the timer
    evtimer_add_periodic(&periodic_timer, &periodic_event, xtimer_ticks(PERIODIC_INTERVAL));

    // Main loop to keep the application running
    while (1) {
        // The periodic callback will be triggered automatically
        xtimer_sleep(1);  // Sleep for 1 second to avoid busy-waiting
    }

    return 0;
}
