
#include <stdio.h>
#include <ztimer.h>
#include <unistd.h>
#include <timex.h>
#include <time_units.h>
#include <xtimer.h>
#include <evtimer_msg.h>

// Define a callback function for the periodic event
void event_periodic_callback(void *arg)
{
    (void)arg;
    printf("Periodic event triggered!\n");
}

int main(void)
{
    // Initialize the event timer
    evtimer_t evtimer;
    evtimer_init_msg(&evtimer);

    // Define a periodic event structure
    evtimer_msg_event_t periodic_event;
    periodic_event.event.handler = event_periodic_callback;
    periodic_event.event.arg = NULL;

    // Set the periodic event to trigger every second
    evtimer_msg_set(&evtimer, &periodic_event, 1 * US_PER_SEC);

    // Start the periodic event
    evtimer_msg_start(&evtimer);

    // Keep the main thread alive
    while (1) {
        xtimer_sleep(1);
    }

    return 0;
}
