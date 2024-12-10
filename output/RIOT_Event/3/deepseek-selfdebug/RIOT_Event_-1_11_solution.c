
#include "xtimer.h"
#include "event/callback.h"
#include "event/periodic_callback.h"
#include "ztimer.h"

#define INTERVAL_SEC (1U)

static void periodic_callback(void *arg) {
    (void)arg;
    printf("Periodic callback triggered!\n");
}

int main(void) {
    event_queue_t queue;
    event_periodic_callback_t event;

    event_queue_init(&queue);

    event_periodic_callback_init(&event, ZTIMER_SEC, &queue, periodic_callback, NULL);
    event_periodic_callback_start(&event, INTERVAL_SEC);

    while (1) {
        event_loop(&queue);
    }

    return 0;
}
