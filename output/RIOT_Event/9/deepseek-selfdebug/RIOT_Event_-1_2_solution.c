
#include "xtimer.h"
#include "event/periodic.h"
#include "event/callback.h"

#define PERIODIC_INTERVAL (1 * 1000 * 1000U)  // 1 second in microseconds

static void periodic_callback(void *arg) {
    (void)arg;  // Unused parameter
    printf("Periodic event triggered!\n");
}

int main(void) {
    event_periodic_t periodic_event;
    event_periodic_init(&periodic_event, xtimer_usec_from_ticks(PERIODIC_INTERVAL), periodic_callback, NULL);
    event_periodic_start(&periodic_event);

    return 0;
}
