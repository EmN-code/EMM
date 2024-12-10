#include <stdio.h>
#include <sys/time.h>
#include <time.h>
#include <timex.h>
#include <ztimer.h>
#include <evtimer.h>

// Define the frequency of the event (1 event per second)
#define EVENT_FREQ (1U * US_PER_SEC)

static evtimer_t evtimer; // Use evtimer_t for event timer

static void event_periodic_callback(void *arg)
{
    (void)arg; // Silently ignore unused parameter warning
    printf("Periodic event triggered\n");
}

int main(void) {
    evtimer_event_t event; // Declare proper event type

    // Initialize evtimer
    evtimer_init(&evtimer, event_periodic_callback);

    // Set up event
    event.callback = event_periodic_callback;

    if (evtimer_add(&evtimer, &event, EVENT_FREQ) != 0) {
        printf("Failed to add event to evtimer\n");
        return 1;
    }

    // Dispatch the event in a loop
    while (1) {
        evtimer_handle_next_event(&evtimer);
        ztimer_sleep(ZTIMER_MSEC, MS_PER_SEC);
    }

    return 0;
}