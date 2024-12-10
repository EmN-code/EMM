
#include "event/callback.h"
#include "ztimer.h"
#include "xtimer.h"

#define INTERVAL_US (1 * US_PER_SEC)  // 1 second interval

static void callback(void *arg) {
    (void)arg;  // Unused argument
    printf("Periodic callback triggered\n");
}

int main(void) {
    event_periodic_callback_t event;
    event_queue_t queue;

    // Initialize the event queue
    msg_init_queue(NULL, 0);

    // Initialize the periodic callback event
    event_periodic_callback_init(&event, ZTIMER_USEC, &queue, callback, NULL);

    // Start the periodic callback event with a 1-second interval
    event_periodic_callback_start(&event, INTERVAL_US);

    // Main loop to keep the application running
    while (1) {
        msg_receive(NULL);
    }

    return 0;
}
