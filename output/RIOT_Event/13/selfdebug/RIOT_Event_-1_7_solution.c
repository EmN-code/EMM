#include <ztimer.h>
#include <xtimer.h>  // Ensure inclusion of xtimer for related timing functionalities
#include <stdio.h>

#define INTERVAL (1U * MS_PER_SEC)  // 1 second interval

static ztimer_t timer;

// Callback function that gets executed every second
void event_periodic(void *arg) {
    (void) arg;
    printf("Event triggered every second!\n");
    ztimer_set(ZTIMER_MSEC, &timer, INTERVAL);  // Reschedule the timer
}

int main(void) {
    printf("Starting periodic event example with ztimer on ESP32\n");

    // Initialize the timer and set the interval for the first time
    ztimer_set_cb(ZTIMER_MSEC, &timer, INTERVAL, event_periodic, NULL);

    // Start the timer
    ztimer_set(ZTIMER_MSEC, &timer, INTERVAL);

    // Main loop doing nothing, just waiting for callbacks
    while (1) {
        ztimer_sleep(ZTIMER_MSEC, INTERVAL);
    }

    return 0;
}