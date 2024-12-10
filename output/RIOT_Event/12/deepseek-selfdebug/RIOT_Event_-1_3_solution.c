
#include "xtimer.h"
#include "event/periodic.h"

#define INTERVAL_MS (1000U)  // 1 second interval

static void periodic_callback(void *arg) {
    (void)arg;  // Unused argument
    printf("Periodic event triggered!\n");
}

int main(void) {
    event_periodic_t periodic_event;
    event_periodic_event_t periodic_event_data;

    // Initialize the periodic event
    event_periodic_init(&periodic_event, ZTIMER_MSEC, INTERVAL_MS, periodic_callback, NULL);

    // Add the event to the periodic event handler
    event_periodic_add_event(&periodic_event, &periodic_event_data);

    // Start the periodic event
    event_periodic_start(&periodic_event);

    // Keep the main thread alive
    while (1) {
        xtimer_sleep(1);
    }

    return 0;
}
