
#include "xtimer.h"
#include "evtimer_mbox.h"
#include "periph/rtc.h"
#include "evtimer_msg.h"
#include "time_units.h"
#include "ztimer.h"
#include "event/thread.h"
#include "timex.h"
#include "thread.h"
#include "event/periodic_callback.h"

#define INTERVAL_MS (1000U)  // 1 second interval

static void periodic_callback(void *arg)
{
    (void)arg;  // Unused parameter
    printf("Periodic callback triggered!\n");
}

int main(void)
{
    // Initialize the event queue
    event_queue_t event_queue;
    event_queue_init(&event_queue);

    // Initialize the periodic callback event
    event_periodic_callback_t periodic_event;
    event_periodic_callback_init(&periodic_event, ZTIMER_MSEC, &event_queue, periodic_callback, NULL);

    // Start the periodic callback with the specified interval
    event_periodic_callback_start(&periodic_event, INTERVAL_MS);

    // Run the event loop
    while (1) {
        event_process(&event_queue);
    }

    return 0;
}
