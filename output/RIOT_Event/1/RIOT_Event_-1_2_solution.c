
#include "time_units.h"
#include "evtimer_mbox.h"
#include "thread.h"
#include "timex.h"
#include "evtimer_msg.h"
#include "event/periodic_callback.h"
#include "xtimer.h"
#include "ztimer.h"
#include "event/thread.h"

#define INTERVAL_SEC (1U)  // 1 second interval

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

    // Start the periodic callback event with a 1-second interval
    event_periodic_callback_start(&event, INTERVAL_SEC * US_PER_SEC);

    // Main loop to process events
    while (1) {
        event_t *ev = event_wait(&queue);
        if (ev) {
            ev->handler(ev);
        }
    }

    return 0;
}
