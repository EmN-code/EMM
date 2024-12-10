#include <xtimer.h>
#include <stdio.h>
#include <event/periodic.h>

static void callback(void *arg) {
    puts("Periodic event triggered");
}

int main(void) {
    puts("Starting periodic event");

    event_periodic_t periodic_event;
    event_periodic_init(&periodic_event, EVENT_PRIO_MEDIUM, callback, NULL, 1 * US_PER_SEC);
    event_periodic_start(&periodic_event);

    while (1) {
        event_dispatch(EVENT_PRIO_MEDIUM);
    }

    return 0;
}