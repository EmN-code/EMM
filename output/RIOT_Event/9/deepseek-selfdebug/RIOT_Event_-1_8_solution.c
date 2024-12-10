
#include "xtimer.h"
#include "event/callback.h"

#define INTERVAL_MS (1000U)  // 1 second interval

static void periodic_callback(void *arg) {
    (void)arg;  // Unused argument
    printf("Periodic callback triggered!\n");
}

int main(void) {
    event_periodic_t periodic_event;
    event_periodic_init(&periodic_event, periodic_callback, NULL, INTERVAL_MS);
    event_periodic_set(&periodic_event, INTERVAL_MS);
    event_periodic_start(&periodic_event);

    while (1) {
        // Main loop
    }

    return 0;
}
