
#include "event.h"
#include "ztimer.h"
#include "thread.h"
#include "xtimer.h"

#define INTERVAL_MS (1000U)  // 1 second interval

static event_queue_t event_queue;
static event_periodic_callback_t periodic_event;

static void periodic_callback(void *arg) {
    (void)arg;
    printf("Periodic event triggered!\n");
}

int main(void) {
    // Initialize the event queue
    event_queue_init(&event_queue);

    // Initialize the periodic event
    event_periodic_callback_init(&periodic_event, ZTIMER_MSEC, &event_queue, periodic_callback, NULL);

    // Set the interval for the periodic event
    event_periodic_callback_set_interval(&periodic_event, INTERVAL_MS);

    // Start the periodic event
    event_periodic_callback_start(&periodic_event);

    // Main loop to keep the thread running
    while (1) {
        event_loop(&event_queue);
    }

    return 0;
}
