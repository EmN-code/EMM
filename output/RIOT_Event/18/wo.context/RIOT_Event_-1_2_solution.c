#include <ztimer.h>
#include <event/callback.h>

// Define constants
#define TIMER_INTERVAL (1 * ZTIMER_MSEC)  // 1 second in milliseconds

// Function prototype declaration
event_periodic_callback(void *arg);

// Callback function to be executed every second
void event_periodic_callback(void *arg) {
    (void)arg;  // Avoid unused parameter warning
    // Implement the functionality to be repeated every second
    puts("Event occurred");
}

int main(void) {
    printf("Initializing periodic event example on ESP32.\n");

    // Event timer structure
    evtimer_t evtimer;

    // Initialize the event timer
    evtimer_init(&evtimer, event_periodic_callback);

    // Start the timer with a 1-second interval
    evtimer_set_periodic(&evtimer, TIMER_INTERVAL);

    // Keep running
    while (1) {
        // Wait to allow timer callback to operate
        ztimer_sleep(ZTIMER_MSEC, TIMER_INTERVAL);
    }
    return 0;
}