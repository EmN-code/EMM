#include <stdio.h>
#include "xtimer.h"
#include "thread.h"

#define INTERVAL (1U * US_PER_SEC)

// Callback function to be called periodically
timer_t periodic_timer;
void event_periodic(void *arg) {
    (void) arg;
    printf("Periodic event triggered!\n");
    xtimer_set(&periodic_timer, INTERVAL);  // Reschedule the timer
}

int main(void) {
    puts("RIOT Periodic Event Example on ESP32\n");
    
    // Initialize the timer
    xtimer_init();
    
    // Set the timer for the first time
    xtimer_set(&periodic_timer, INTERVAL);
    
    // Event loop
    while (1) {
        event_periodic(NULL);  // Manually call the initial event
        xtimer_sleep(1);  // Sleep for a second until the next
    }

    return 0;
}