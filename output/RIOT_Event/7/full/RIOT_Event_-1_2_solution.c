#include <xtimer.h>  // Required for xtimer functionality
#include <ztimer.h> // For using ztimer related functionalities
#include <timex.h>  // For time-related functionalities
#include <time_units.h>  // For handling time units
#include <evtimer_msg.h>  // Provides evtimer with msg handling
#include <evtimer_mbox.h>  // Provides evtimer with mbox handling
#include <stdio.h>  // Required for input/output functions

#define INTERVAL (1U * US_PER_SEC)  // Set interval for 1 second using microseconds

// Periodic event action callback
static void my_periodic_callback(void *arg)
{
    (void)arg;
    printf("Periodic event triggered every 1 second.\n");
}

int main(void)
{
    evtimer_t evtimer;

    // Initialize evtimer using mailbox handler
    evtimer_init_mbox(&evtimer);

    // Initialize periodic event structure
    evtimer_periodic_t periodic_event;
    periodic_event.callback = my_periodic_callback;
    periodic_event.period = INTERVAL;
    periodic_event.arg = NULL;

    // Add periodic event with set interval
    evtimer_add_periodic(&evtimer, &periodic_event, INTERVAL);

    // Simulate waiting for events
    while (1) {
        ztimer_sleep(ZTIMER_MSEC, INTERVAL);
    }

    return 0;
}
