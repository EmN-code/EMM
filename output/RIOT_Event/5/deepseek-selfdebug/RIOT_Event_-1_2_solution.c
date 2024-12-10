
#include "event/periodic.h"
#include "ztimer.h"
#include "thread.h"

#define INTERVAL_MS (1000U)  // 1 second interval

static void periodic_callback(void *arg) {
    (void)arg;  // Unused argument
    printf("Periodic callback triggered!\n");
}

int main(void) {
    event_periodic_callback_t event;
    event_queue_t queue;

    // Initialize the event queue
    msg_init_queue(NULL, 0);

    // Initialize the periodic callback event
    event_periodic_callback_init(&event, ZTIMER_MSEC, &queue, periodic_callback, NULL);

    // Start the periodic callback event with the specified interval
    event_periodic_callback_start(&event, INTERVAL_MS);

    // Main loop to keep the thread alive
    while (1) {
        thread_yield();
    }

    return 0;
}
