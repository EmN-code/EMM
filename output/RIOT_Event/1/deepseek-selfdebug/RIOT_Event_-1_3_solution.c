
#include <stdio.h>
#include "event/periodic.h"
#include "ztimer.h"
#include "thread.h"

#define INTERVAL (1 * MS_PER_SEC)  // 1 second interval

static void callback(void *arg) {
    printf("Periodic callback triggered!\n");
}

int main(void) {
    event_periodic_callback_t event;
    event_queue_t queue;

    // Initialize the event queue
    event_queue_init(&queue);

    // Initialize the periodic callback event
    event_periodic_callback_init(&event, ZTIMER_USEC, &queue, callback, NULL);

    // Start the periodic callback event with a 1 second interval
    event_periodic_callback_start(&event, INTERVAL);

    // Main loop to keep the thread running
    while (1) {
        thread_yield();
    }

    return 0;
}
