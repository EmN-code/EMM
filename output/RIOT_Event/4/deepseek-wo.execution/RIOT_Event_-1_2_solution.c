
#include "time_units.h"
#include "ztimer.h"
#include "thread.h"
#include "xtimer.h"
#include "event/thread.h"
#include "timex.h"
#include "event/periodic_callback.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"

#define INTERVAL_SEC (1U)

static event_queue_t event_queue;
static event_periodic_callback_t periodic_event;

static void periodic_callback(void *arg)
{
    (void)arg;
    printf("Periodic callback triggered!\n");
}

int main(void)
{
    event_queue_init(&event_queue);

    // Initialize the periodic callback event
    event_periodic_callback_init(&periodic_event, ZTIMER_MSEC, &event_queue, periodic_callback, NULL);

    // Start the periodic callback event with a 1-second interval
    event_periodic_callback_start(&periodic_event, INTERVAL_SEC * MS_PER_SEC);

    // Run the event loop
    while (1) {
        event_loop(&event_queue);
    }

    return 0;
}
