#include <stdio.h>       // For printf function
#include <stdlib.h>      // For standard library functions
#include <atomic.h>      // For atomic operations
#include <stdbool.h>     // For boolean types

#include "esp_wifi.h"   // ESP Wi-Fi functionalities

// Define a structure for the event timer
typedef struct {
    unsigned int timer;     // Simulated timer
    atomic_bool active;     // Flag to indicate if the timer is active
} my_event_timer_t;

// Callback function to be called when the timer expires
void my_callback_function(void *arg) {
    printf("Callback function called every second.\n");
}

// Function to initialize the timer
void my_event_timer_init(my_event_timer_t *evtimer) {
    evtimer->timer = 0;  // Reset the timer
    atomic_store(&evtimer->active, true);
}

// Function to set up a periodic event callback
void my_event_timer_set(my_event_timer_t *evtimer) {
    while (atomic_load(&evtimer->active)) {
        if (++evtimer->timer >= 1000) {  // Simulate 1 second intervals
            evtimer->timer = 0;  // Reset simulated timer
            my_callback_function(NULL);  // Trigger callback
        }
    }
}

int main(void) {
    my_event_timer_t evtimer;
    my_event_timer_init(&evtimer);

    // Start the periodic event
    my_event_timer_set(&evtimer);
    
    return 0;
}
