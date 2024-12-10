
#include "event/callback.h"
#include "event/periodic_callback.h"
#include "event/thread.h"
#include "ztimer.h"
#include "xtimer.h"

#define INTERVAL_MS (1000U)  // 1 second interval

static event_queue_t event_queue;
static event_periodic_callback_t periodic_event;

static void periodic_callback(void *arg)
{
    (void)arg;
    printf("Periodic callback triggered!\n");
}

int main(void)
{
    // Initialize the event queue
    event_queue_init(&event_queue);

    // Initialize the periodic event
    event_periodic_callback_create(&periodic_event, ZTIMER_MSEC, INTERVAL_MS, &event_queue, periodic_callback);

    // Start the periodic event
    event_periodic_callback_start(&periodic_event, INTERVAL_MS);

    // Dispatch events from the event queue
    while (1) {
        event_queue_dispatch(&event_queue);
    }

    return 0;
}
