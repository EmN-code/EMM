// In this code, we will initialize a periodic event handler using RIOT's event library.
// We will set up a callback that triggers every 1 second using the `evtimer` module.

#include "xtimer.h"  // Provides access to xtimer, RIOT's high-precision timer
#include "sched.h"   // Includes functions for task and scheduler management
#include "event/callback.h"  // Provides the structures and functions for working with events
#include <stdio.h>    // Includes the standard I/O library

// Define the frequency of our periodic event in microseconds
#define PERIOD (1000000)  // 1 second

// Event callback function that will be triggered periodically
event_callback_t callback;

int counter = 0; // To keep track of callback executions

// Function to be called at each event occurrence
void periodic_callback(void *arg) {
    (void)arg; // Unused parameter
    printf("Callback executed %d times\n", ++counter);
}

int main(void) {
    printf("Starting periodic event example on ESP32\n");

    // Initialize the event callback with the periodic function and NULL arguments
    callback = EVENT_CALLBACK_INIT(periodic_callback, NULL);

    // Set the callback to repeat at the defined period
    evtimer_t evtimer;
    evtimer_init(&evtimer, &callback);
    evtimer_set(&evtimer, PERIOD);

    // Keep the main function running indefinitely
    while (1) {
        // Allow other threads to process
        xtimer_usleep(PERIOD);
    }

    return 0;
}