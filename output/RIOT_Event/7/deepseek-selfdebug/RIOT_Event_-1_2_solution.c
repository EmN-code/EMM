
#include "xtimer.h"
#include "event/callback.h"
#include "event/periodic.h"

#define INTERVAL_MS (1000U)  // 1 second interval

static void periodic_callback(void *arg) {
    (void)arg;  // Unused parameter
    printf("Periodic callback triggered!\n");
}

int main(void) {
    event_periodic_t periodic_event;
    event_periodic_init(&periodic_event, xtimer_now_usec64(), INTERVAL_MS * 1000, periodic_callback, NULL);
    event_periodic_add(&periodic_event);

    while (1) {
        // Main loop
    }

    return 0;
}
