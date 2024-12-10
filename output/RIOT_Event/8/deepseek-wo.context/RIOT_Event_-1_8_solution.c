
#include <stdio.h>
#include "xtimer.h"
#include "event/callback.h"

#define INTERVAL_MS (1000 * 1000U)  // 1 second in microseconds

static evtimer_t periodic_timer;
static evtimer_msg_event_t periodic_event;

static void periodic_callback(void *arg)
{
    (void)arg;
    printf("Periodic event triggered!\n");
}

int main(void)
{
    // Initialize the event timer
    evtimer_init(&periodic_timer);

    // Set up the periodic event
    periodic_event.event.callback = periodic_callback;
    periodic_event.event.arg = NULL;

    // Add the periodic event to the timer
    evtimer_add_msg(&periodic_timer, &periodic_event, INTERVAL_MS);

    // Main loop to keep the application running
    while (1) {
        xtimer_sleep(1);
    }

    return 0;
}
