
#include "xtimer.h"
#include "event/callback.h"
#include "event/timeout.h"

#define PERIODIC_INTERVAL (1 * US_PER_SEC)  // 1 second interval

static void periodic_callback(void *arg)
{
    (void)arg;  // Unused argument
    printf("Periodic event triggered!\n");
}

int main(void)
{
    // Initialize the event timer
    evtimer_t event_timer;
    evtimer_init(&event_timer);

    // Create an event for the periodic callback
    evtimer_event_t periodic_event;
    periodic_event.callback = periodic_callback;
    periodic_event.arg = NULL;

    // Set the periodic event to trigger every second
    xtimer_t periodic_timer;
    periodic_timer.callback = (xtimer_callback_t)evtimer_add;
    periodic_timer.arg = &periodic_event;

    // Start the periodic timer
    xtimer_set(&periodic_timer, PERIODIC_INTERVAL);

    // Main loop to keep the application running
    while (1) {
        // Process events
        xtimer_usleep(100000);  // Sleep for 100ms to avoid busy-waiting
    }

    return 0;
}
