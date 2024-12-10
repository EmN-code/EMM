
#include "xtimer.h"
#include "event/periodic.h"
#include "event/callback.h"

#define PERIOD_US (1000000U)  // 1 second in microseconds

static void periodic_callback(void *arg) {
    (void)arg;  // Unused argument
    printf("Periodic callback triggered!\n");
}

int main(void) {
    // Initialize the periodic event
    event_periodic_t periodic_event;
    event_periodic_callback_init(&periodic_event, xtimer_ticks64(PERIOD_US), periodic_callback, NULL);

    // Start the periodic event
    event_periodic_callback_start(&periodic_event);

    // Keep the main thread alive
    while (1) {
        xtimer_sleep(1);
    }

    return 0;
}
