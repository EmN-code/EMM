
#include "event/callback.h"
#include "ztimer.h"
#include "thread.h"

#define INTERVAL (1 * US_PER_SEC)  // 1 second interval

static event_periodic_callback_t periodic_event;

void periodic_callback(void *arg)
{
    (void)arg;  // Unused argument
    printf("Periodic callback triggered!\n");
}

int main(void)
{
    event_queue_t event_queue = { NULL };
    ztimer_clock_t *clock = ZTIMER_USEC;

    // Initialize the event queue
    msg_init_queue(event_queue.queue, EVENT_QUEUE_SIZE);

    // Initialize and start the periodic callback event
    event_periodic_callback_init(&periodic_event, clock, &event_queue, periodic_callback, NULL);
    event_periodic_callback_start(&periodic_event, INTERVAL);

    // Main loop to keep the thread alive
    while (1) {
        thread_yield();
    }

    return 0;
}
