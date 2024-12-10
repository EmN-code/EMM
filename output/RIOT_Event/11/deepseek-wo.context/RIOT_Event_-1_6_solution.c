
#include "xtimer.h"
#include "event/periodic.h"
#include "event/callback.h"
#include "event/thread.h"

#define INTERVAL_MS (1000U)  // 1 second interval

static event_periodic_t periodic_event;
static event_queue_t event_queue;

static void periodic_callback(void *arg)
{
    (void)arg;
    printf("Periodic event triggered!\n");
}

int main(void)
{
    // Initialize the event queue
    event_queue_init(&event_queue);

    // Initialize the periodic event
    event_periodic_init(&periodic_event, xtimer_ticks32(INTERVAL_MS * US_PER_MS), periodic_callback, NULL);

    // Add the periodic event to the event queue
    event_periodic_add(&event_queue, &periodic_event);

    // Start the event thread
    event_thread_create(&event_queue, "event_thread", EVENT_THREAD_STACKSIZE, EVENT_THREAD_PRIO);

    return 0;
}
