
#include <stdio.h>
#include "xtimer.h"
#include "event.h"  // Include the event header file

#define INTERVAL_MS (1000U)  // 1 second interval

static event_queue_t event_queue;
static xtimer_t timer;

void event_periodic_callback(void *arg) {
    (void)arg;  // Unused parameter
    printf("Periodic event callback triggered!\n");
}

int main(void) {
    // Initialize the event queue
    event_queue_init(&event_queue);

    // Initialize the event thread
    event_thread_init(&event_queue, NULL, 0, THREAD_PRIORITY_MAIN - 1);

    // Set up the periodic timer
    timer.callback = event_periodic_callback;
    timer.arg = NULL;
    xtimer_set(&timer, INTERVAL_MS * US_PER_MS);

    // Main loop to keep the application running
    while (1) {
        // Process events
        event_process(&event_queue);
    }

    return 0;
}
