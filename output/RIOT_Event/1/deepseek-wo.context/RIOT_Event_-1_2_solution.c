
#include "event/periodic.h"
#include "ztimer.h"
#include "thread.h"

#define INTERVAL (1 * US_PER_SEC)  // Define the interval in microseconds

static void callback(void *arg) {
    printf("Periodic callback triggered!\n");
}

int main(void) {
    event_periodic_callback_t event;
    event_queue_t queue;

    // Initialize the event queue
    msg_init_queue(NULL, 0);

    // Initialize the periodic callback event
    event_periodic_callback_init(&event, ZTIMER_USEC, &queue, callback, NULL);

    // Start the periodic callback event with the defined interval
    event_periodic_callback_start(&event, INTERVAL);

    // Main loop to keep the application running
    while (1) {
        thread_yield();
    }

    return 0;
}
