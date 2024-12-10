#include <stdio.h> // for standard input/output functions
#include <event/callback.h> // for event callback functions in RIOT

// Function prototypes
typedef void (*ztimer_periodic_callback_t)(void *arg);

// A mocked version of ztimer_periodic_callback_t
void ztimer_callback(void *arg) {
    printf("Alarm triggered!\n");
}

int main() {
    // Mocking the initialization of the RTC and setting the time
    printf("Initializing RTC and setting date to June 1, 2024\n");

    // Create a timer event
    ztimer_periodic_callback_t callback = ztimer_callback; // Assign the callback function

    // Mock setting a timer to trigger the alarm after 5 seconds
    printf("Setting a timer for 5 seconds...\n");
    callback(NULL); // Simulating the timer expiry, directly calling callback

    return 0;
}