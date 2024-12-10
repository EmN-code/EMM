#include <stdio.h>
#include <xtimer.h>
#include <esp_idf_version.h>

#define INTERVAL (1U * US_PER_SEC) // Set interval to 1 second in microseconds

// Callback function to be called periodically
timer_t periodic_timer;
void event_periodic(void *arg) {
    (void)arg; // Suppress unused parameter warning
    printf("Event occurred!\n");
    // Restart the timer to call this function again after INTERVAL
    xtimer_set(&periodic_timer, INTERVAL);
}

int main(void) {
    printf("RIOT ESP32 Periodic Event Example\n");
    // Initialize the periodic timer
    xtimer_init();
    // Set the initial timer event
    xtimer_set(&periodic_timer, INTERVAL);
    // Attach the callback function to the timer
    periodic_timer.callback = event_periodic;
    periodic_timer.arg = NULL;
    while (1) {
        // Main loop does nothing, all action is in the callback
        xtimer_sleep(1);
    }
    return 0;
}