
#include "event_periodic_callback.h"
#include "ztimer.h"
#include "event.h"

#define INTERVAL_SEC (1U)

static void periodic_callback(void *arg)
{
    (void)arg;
    printf("Periodic callback triggered!\n");
}

int main(void)
{
    event_periodic_callback_t event;
    event_queue_t event_queue = EVENT_QUEUE_INIT(event_queue);

    /* Initialize the periodic callback event */
    event_periodic_callback_init(&event, ZTIMER_USEC, INTERVAL_SEC * US_PER_SEC, &event_queue, periodic_callback, NULL);

    /* Start the periodic callback event */
    event_periodic_callback_start(&event);

    /* Run the event loop */
    while (1) {
        event_loop(&event_queue);
    }

    return 0;
}
