#include <ztimer.h>
#include <stdio.h>
#include <inttypes.h>

// Define a callback function
void my_event_callback(void *arg) {
    puts("Event triggered!");
}

int main(void) {
    // Define and initialize a ztimer, which is used in RIOT for timing events
    ztimer_t periodic_timer;
    periodic_timer.callback = my_event_callback; 
    periodic_timer.arg = NULL;

    // Start periodic timer with an interval of 1 second (1,000,000 us)
    ztimer_periodic_init(ZTIMER_MSEC, &periodic_timer, 1000);
    
    // Main loop
    while (1) {
        ztimer_sleep(ZTIMER_MSEC, 1000); // Sleep for 1 second
    }

    return 0;
}
