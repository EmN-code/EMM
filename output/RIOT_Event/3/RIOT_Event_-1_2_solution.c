
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

static event_queue_t event_queue;
static event_periodic_callback_t periodic_event;

static void periodic_callback(void *arg)
{
    (void)arg;
    puts("Periodic callback triggered!");
}

int main(void)
{
    // Initialize the event queue
    event_queue_init(&event_queue);

    // Initialize the periodic callback event
    event_periodic_callback_init(&periodic_event, ZTIMER_USEC, &event_queue, periodic_callback, NULL);

    // Start the periodic callback event with a 1 second interval
    event_periodic_callback_start(&periodic_event, INTERVAL_SEC * US_PER_SEC);

    // Main loop to process events
    while (1) {
        event_t event;
        event_queue_get(&event_queue, &event);
        event_handle(&event);
    }

    return 0;
}
