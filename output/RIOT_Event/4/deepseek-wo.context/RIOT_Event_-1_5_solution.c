
#include <stdio.h>
#include "xtimer.h"
#include "event/callback.h"
#include "event/periodic_callback.h"

void callback(void *arg) {
    printf("Periodic callback triggered!\n");
}

int main(void) {
    event_periodic_callback_t event;
    event_queue_t queue;

    // Initialize the event queue
    msg_init_queue(queue.queue, sizeof(queue.queue) / sizeof(queue.queue[0]));

    // Initialize the periodic callback event
    event_periodic_callback_init(&event, ZTIMER_MSEC, &queue, callback);

    // Set the interval to 1 second
    event_periodic_callback_set_interval(&event, 1000);

    // Start the periodic callback event
    event_periodic_callback_start(&event);

    // Keep the main thread alive
    while (1) {
        xtimer_sleep(1);
    }

    return 0;
}
