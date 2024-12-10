
#include "evtimer_mbox.h"
#include "ztimer.h"
#include "timex.h"
#include "time_units.h"
#include "xtimer.h"
#include "evtimer_msg.h"

#define PERIODIC_INTERVAL (1 * US_PER_SEC)  // 1 second interval

static evtimer_t evtimer;
static evtimer_event_t event;

static void event_periodic_callback(void *arg)
{
    (void)arg;
    printf("Periodic event triggered!\n");
}

int main(void)
{
    // Initialize the event timer
    evtimer_init(&evtimer);

    // Set up the event
    event.event.handler = event_periodic_callback;
    event.event.arg = NULL;

    // Add the event to the timer with a periodic interval
    evtimer_add_event(&evtimer, &event, PERIODIC_INTERVAL);

    // Main loop to keep the application running
    while (1) {
        xtimer_sleep(1);  // Sleep for 1 second to keep the loop running
    }

    return 0;
}
