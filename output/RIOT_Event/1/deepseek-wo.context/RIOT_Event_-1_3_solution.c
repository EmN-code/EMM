
#include <stdio.h>
#include "xtimer.h"
#include "event/periodic.h"
#include "event/callback.h"

#define INTERVAL_US (1 * US_PER_SEC)

static event_periodic_callback_t periodic_event;

static void periodic_callback(void *arg) {
    printf("Periodic callback triggered!\n");
}

int main(void) {
    event_queue_t event_queue = EVENT_QUEUE_INIT(event_queue);

    event_periodic_callback_init(&periodic_event, &xtimer_clock, &event_queue, periodic_callback, NULL);
    event_periodic_callback_set_interval(&periodic_event, INTERVAL_US);
    event_periodic_callback_start(&periodic_event);

    while (1) {
        event_loop(&event_queue);
    }

    return 0;
}
