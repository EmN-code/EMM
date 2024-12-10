
#include "xtimer.h"
#include "event/periodic.h"
#include "event/callback.h"

#define INTERVAL_MS (1000U)  // 1 second interval

static void periodic_callback(void *arg) {
    (void)arg;  // Unused argument
    printf("Periodic callback triggered!\n");
}

int main(void) {
    event_periodic_t periodic_event;
    event_periodic_event_t event;

    // Initialize the periodic event with the callback function and interval
    event_periodic_init(&periodic_event, xtimer_ticks64(xtimer_ms_to_ticks(INTERVAL_MS)), periodic_callback, NULL);

    // Initialize the event to be added to the periodic event
    event_periodic_event_init(&event, periodic_callback, NULL);

    // Add the event to the periodic event
    event_periodic_add_event(&periodic_event, &event);

    // Start the periodic event
    event_periodic_start(&periodic_event);

    // Keep the main thread alive
    while (1) {
        xtimer_sleep(1);
    }

    return 0;
}
