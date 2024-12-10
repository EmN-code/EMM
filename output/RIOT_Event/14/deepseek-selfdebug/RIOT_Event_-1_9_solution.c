
#include "xtimer.h"
#include "event.h"
#include "thread.h"

#define INTERVAL_MS (1000U)  // 1 second interval

static void periodic_event_cb(void *arg) {
    (void)arg;  // Unused argument
    printf("Periodic event callback triggered!\n");
}

int main(void) {
    // Initialize the periodic event
    event_periodic_t periodic_event;
    event_periodic_init(&periodic_event, xtimer_ticks32(INTERVAL_MS * US_PER_MS), periodic_event_cb, NULL);

    // Start the periodic event
    event_periodic_start(&periodic_event);

    // Keep the main thread alive
    while (1) {
        thread_yield();
    }

    return 0;
}
