
#include "xtimer.h"
#include "event/periodic_callback.h"
#include "event/callback.h"
#include "event/thread.h"

#define INTERVAL_MS (1000U)  // 1 second interval

static void periodic_callback(void *arg)
{
    (void)arg;
    printf("Periodic callback triggered!\n");
}

int main(void)
{
    event_periodic_callback_t event;
    ztimer_clock_t *clock = ZTIMER_USEC;
    event_queue_t *queue = &event_loop_queue;

    // Initialize the periodic callback event
    event_periodic_callback_init(&event, clock, queue, periodic_callback);

    // Set the interval for the periodic callback
    event_periodic_callback_set_interval(&event, INTERVAL_MS);

    // Start the periodic callback
    event_periodic_callback_start(&event);

    // Run the event loop
    event_loop();

    return 0;
}
