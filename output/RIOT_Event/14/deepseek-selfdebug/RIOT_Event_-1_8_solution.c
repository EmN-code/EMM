
#include "xtimer.h"
#include "event/periodic.h"

#define INTERVAL_MS (1000U)  // 1 second interval

static void periodic_callback(void *arg) {
    (void)arg;  // Unused argument
    printf("Periodic event triggered!\n");
}

int main(void) {
    event_periodic_t periodic_event;
    xtimer_ticks32_t interval = xtimer_ticks32_from_ms(INTERVAL_MS);

    // Initialize the periodic event
    event_periodic_init(&periodic_event, interval, periodic_callback, NULL);

    // Start the periodic event
    event_periodic_start(&periodic_event);

    // Keep the main thread alive
    while (1) {
        xtimer_sleep(1);
    }

    return 0;
}
