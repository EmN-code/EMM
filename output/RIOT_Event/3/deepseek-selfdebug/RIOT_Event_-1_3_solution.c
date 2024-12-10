
#include <stdio.h>
#include "event/periodic.h"
#include "ztimer.h"
#include "thread.h"

#define INTERVAL (1 * US_PER_SEC)  // 1 second interval

static void callback(void *arg) {
    printf("Periodic callback triggered!\n");
}

int main(void) {
    event_periodic_callback_t event;
    event_queue_t queue;

    // Initialize the event queue
    msg_init_queue(queue.queue, sizeof(queue.queue));

    // Initialize the periodic callback event
    event_periodic_callback_init(&event, ZTIMER_USEC, &queue, callback, NULL);

    // Start the periodic callback event with a 1 second interval
    event_periodic_callback_start(&event, INTERVAL);

    // Process events
    while (1) {
        event_process(&queue);
    }

    return 0;
}
