#include <stdio.h>
#include <event/periodic.h>

void periodic_callback(void *arg) {
    // This function will be called every second.
    printf("Periodic callback is triggered.\n");
}

int main(void) {
    printf("Initializing periodic event...\n");
    
    evtimer_t periodic_timer;
    evtimer_init(&periodic_timer, periodic_callback);
    
    // Set the timer to be periodic every 1000ms (or 1 second)
    evtimer_set_periodic(&periodic_timer, 1000);
    
    // Start the timer
    evtimer_start(&periodic_timer);
    
    // Main loop
    while (1) {
        // This loop can be used to handle other tasks
        // For now, it just sleeps to allow periodic tasks to execute
        sleep(1);
    }

    return 0;
}

#include <event/callback.h>