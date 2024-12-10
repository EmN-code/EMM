#include <event/periodic.h> // Include the necessary header for periodic events
#include <event/callback.h> // Include the header for event callbacks

#include <stdio.h> // Standard I/O for logging purposes

// Function prototype for our callback
void callback(void *arg);

// Definition of the callback function
void callback(void *arg) {
    (void)arg; // We are not using any argument in this callback, so explicitly mark it as unused
    printf("Callback triggered every second!\n");
}

int main(void) {
    // Initialize an evtimer variable
    evtimer_t evtimer;

    // Initialize the periodic event
    evtimer_init(&evtimer, callback);

    // Start the event to invoke callback every 1 second
    evtimer_set(&evtimer, 1000, callback, NULL);

    // Start the event loop (in a real scenario, this would likely be more complex)
    while(1) {
        // Simulate waiting for events - in real scenarios, you might enter low-power mode here
    }
    return 0;
}