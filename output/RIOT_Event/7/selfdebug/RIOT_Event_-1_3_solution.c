#include <stdio.h>
#include <stdlib.h>
#include <xtimer.h>
#include <event.h>
#include <event/thread.h>

void periodic_callback(void *arg) {
    (void)arg;
    printf("Periodic event triggered every second\n");
}

int main(void) {
    /* Initialize the event periodic struct */
    event_periodic_t periodic_event;

    /* Set up the periodic event with a callback and an interval of 1 second */
    if (event_periodic_init(&periodic_event, EVENT_PRIO_MAIN, periodic_callback, NULL,
                            xtimer_ticks_from_usec(1000000)) != 0) {
        printf("Failed to initialize periodic event\n");
        return -1;
    }

    /* Start the periodic event */
    if (event_periodic_start(&periodic_event) != 0) {
        printf("Failed to start periodic event\n");
        return -1;
    }

    /* Main loop */
    while (1) {
        /* Wait for and process incoming events */
        event_dispatch(EVENT_PRIO_MAIN);
    }

    return 0;
}