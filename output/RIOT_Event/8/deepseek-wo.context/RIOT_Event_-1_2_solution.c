
#include "xtimer.h"
#include "event/callback.h"
#include "event/periodic.h"

#define PERIODIC_INTERVAL (1 * 1000 * 1000U)  // 1 second in microseconds

static void periodic_callback(void *arg) {
    (void)arg;
    printf("Periodic event triggered!\n");
}

int main(void) {
    event_periodic_t periodic_event;
    event_periodic_init(&periodic_event, xtimer_now(), PERIODIC_INTERVAL, periodic_callback, NULL);
    event_periodic_add(&periodic_event);

    while (1) {
        xtimer_sleep(1);  // Sleep to prevent busy-waiting
    }

    return 0;
}
