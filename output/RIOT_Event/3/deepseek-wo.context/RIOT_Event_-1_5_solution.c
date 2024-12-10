
#include "ztimer.h"
#include "event.h"
#include "thread.h"
#include "xtimer.h"

#define INTERVAL (1 * US_PER_SEC)  // 1 second interval

static void callback(void *arg) {
    (void)arg;
    printf("Periodic callback triggered\n");
}

int main(void) {
    event_queue_t queue;
    event_periodic_callback_t periodic_event;

    event_queue_init(&queue);

    event_periodic_callback_init(&periodic_event, ZTIMER_USEC, &queue, callback, NULL);
    event_periodic_callback_set_interval(&periodic_event, INTERVAL);
    event_periodic_callback_start(&periodic_event);

    while (1) {
        event_queue_dispatch(&queue);
    }

    return 0;
}
