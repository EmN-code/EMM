
#include "xtimer.h"
#include "event.h"
#include "thread.h"

#define PERIODIC_INTERVAL (1 * 1000 * 1000U)  // 1 second in microseconds

static void periodic_callback(void *arg) {
    (void)arg;  // Suppress unused parameter warning
    printf("Periodic event triggered!\n");
}

int main(void) {
    event_queue_t event_queue;
    event_t periodic_event;

    event_queue_init(&event_queue);
    event_init(&periodic_event, periodic_callback, NULL);

    while (1) {
        event_post(&event_queue, &periodic_event);
        xtimer_usleep(PERIODIC_INTERVAL);
    }

    return 0;
}
