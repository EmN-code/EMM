#include <stdio.h>  // Correct for use of printf
#include <event/periodic.h>  // Include the periodic event definitions
#include <kernel_types.h>  // Include kernel type definitions

static void my_periodic_callback(void *arg) {
    printf("Periodic event triggered!\n");
}

int main(void) {
    evtimer_t evtimer;
    evtimer_event_t event;
    ztimer_t timer;
    
    /* Initialize the periodic event timer */
    event_periodic_init(&evtimer, &timer, ZTIMER_MSEC);
    
    /* Set the periodic callback function to be triggered every second */
    evtimer_event_init(&event, my_periodic_callback, NULL);
    event_periodic_add(&evtimer, &event, 1000);

    /* Start the periodic event */
    event_periodic_start(&evtimer);

    return 0;
}