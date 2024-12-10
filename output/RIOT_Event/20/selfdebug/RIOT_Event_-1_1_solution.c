#include <timex.h> 
#include <event/periodic.h>

static void my_callback(void *arg) {
    // Callback function implementation
    printf("Callback has been called!\n");
}

int main(void) {
    /* Define the evtimer variable */
    evtimer_t evtimer;
    
    /* Initialize the periodic event */
    evtimer_periodic_t periodic;
    periodic.interval_s = 1; // 1 second interval
    periodic.offset = timex_set(0, 0);
    periodic.in = 0; // Start immediately
    periodic.callback = my_callback;
    periodic.arg = NULL;
    
    /* Initialize the event timer */
    evtimer_init_msg(&evtimer);
    
    /* Add the periodic event to the event timer */
    evtimer_add_periodic(&evtimer, &periodic);

    /* Main loop */
    while (1) {
        evtimer_periodic_process(&evtimer);
    }

    return 0;
}