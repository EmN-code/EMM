
#include "xtimer.h"
#include "event/callback.h"
#include "event/periodic.h"

static void periodic_callback(void *arg) {
    (void)arg;
    printf("Periodic callback triggered!\n");
}

int main(void) {
    event_periodic_t periodic_event;
    event_periodic_init(&periodic_event, periodic_callback, NULL);
    event_periodic_add_periodic(&periodic_event, xtimer_ticks64(xtimer_s2ticks(1)));

    while (1) {
        xtimer_sleep(1);
    }

    return 0;
}
