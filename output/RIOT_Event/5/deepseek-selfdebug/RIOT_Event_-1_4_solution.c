
#include "ztimer.h"
#include "event.h"
#include "thread.h"

#define INTERVAL (1 * US_PER_SEC)  // 1 second interval

static void periodic_callback(void *arg) {
    (void)arg;
    printf("Periodic callback triggered!\n");
}

int main(void) {
    event_queue_t event_queue;
    event_queue_init(&event_queue);

    ztimer_periodic_callback_t periodic_event;
    ztimer_periodic_callback_init(&periodic_event, ZTIMER_USEC, &event_queue, periodic_callback, NULL);

    ztimer_periodic_callback_set_interval(&periodic_event, INTERVAL);
    ztimer_periodic_callback_start(&periodic_event);

    while (1) {
        event_loop(&event_queue);
    }

    return 0;
}
