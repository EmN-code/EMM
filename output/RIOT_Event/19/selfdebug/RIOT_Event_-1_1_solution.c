#include "event/periodic.h"
#include "event/callback.h"

#include <stdio.h>

// Function that will be called every second
void callback(void *arg) {
    printf("Callback function called every second\n");
}

int main(void) {
    // Initialize a periodic event timer
    event_periodic_t periodic_timer;
    
    // Configure the event to call 'callback' every second
    event_periodic_init(&periodic_timer, 1 * 1000000, callback, NULL);
    
    // Start the periodic event
    event_periodic_start(&periodic_timer);

    // Keep the program running to continuously see callback execution
    while(1) {
        // Can add other tasks here later
    }

    return 0;
}