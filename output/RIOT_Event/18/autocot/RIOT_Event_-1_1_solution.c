#include <stdio.h>
#include <stdint.h>
#include <event/callback.h>
#include <event/periodic.h>

/*
 * This example demonstrates initializing and starting a periodic event timer
 * with RIOT OS. The callback function will be executed every second.
 */

static void periodic_callback(void *arg) {
    (void)arg;  // Avoid unused parameter warning
    printf("Periodic event triggered!\n");
}

int main(void) {
    printf("RIOT Periodic Event Example\n");

    /* Initialize periodic event callback */
    evtimer_periodic_t periodic_event;

    /* Initialize the periodic event - set it to trigger every second (1000000 microseconds) */
    evtimer_periodic_init(&periodic_event, periodic_callback, NULL, 1000000U);

    /* Start the periodic event */
    evtimer_periodic_start(&periodic_event);

    /* Keep the main function running to continuously monitor the periodic event */
    while (1) {
        /* Simulate doing other work in the main loop */
        /* Add a delay or yield to avoid busy-waiting */
    }

    return 0;
}