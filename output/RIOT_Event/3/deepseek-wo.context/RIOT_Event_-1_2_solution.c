
#include "event/callback.h"
#include "ztimer.h"
#include "thread.h"

#define INTERVAL_US (1000000U)  // 1 second in microseconds

static void periodic_callback(void *arg)
{
    (void)arg;  // Unused argument
    printf("Periodic callback triggered!\n");
}

int main(void)
{
    event_periodic_callback_t event;
    event_queue_t queue;

    // Initialize the event queue
    event_queue_init(&queue);

    // Initialize the periodic callback event
    event_periodic_callback_init(&event, ZTIMER_USEC, &queue, periodic_callback, NULL);

    // Start the periodic callback event with an interval of 1 second
    event_periodic_callback_start(&event, INTERVAL_US);

    // Main loop to process events
    while (1) {
        event_process(&queue);
    }

    return 0;
}
