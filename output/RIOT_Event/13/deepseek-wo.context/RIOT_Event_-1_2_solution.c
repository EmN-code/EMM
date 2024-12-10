
#include "xtimer.h"
#include "event.h"
#include "thread.h"

#define PERIODIC_INTERVAL (1 * US_PER_SEC)  // 1 second interval

static void periodic_callback(void *arg) {
    (void)arg;  // Unused argument
    printf("Periodic event triggered!\n");
}

int main(void) {
    event_periodic_t periodic_event;

    // Initialize the periodic event with the callback function and interval
    event_periodic_init(&periodic_event, periodic_callback, NULL, PERIODIC_INTERVAL);

    // Start the periodic event
    event_periodic_start(&periodic_event);

    // Keep the main thread alive
    while (1) {
        xtimer_sleep(1);
    }

    return 0;
}
