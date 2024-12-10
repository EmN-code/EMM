
#include "xtimer.h"
#include "event/callback.h"
#include "event/periodic.h"

static void periodic_callback(void *arg) {
    printf("Periodic callback triggered!\n");
}

int main(void) {
    event_periodic_t periodic_event;
    event_periodic_init(&periodic_event, xtimer_ticks_from_usec(1000000), periodic_callback, NULL);
    event_periodic_add(&periodic_event);

    return 0;
}
