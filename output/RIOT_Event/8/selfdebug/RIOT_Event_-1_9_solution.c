/* Description:
 * This program is designed to run on an ESP32 using RIOT OS.
 * It sets up a periodic timer that triggers every second, invoking the function `event_periodic`.
 * The code specifically handles issues with improper argument types in function calls.
 */

// Required header files
#include <ztimer/ztimer.h>
#include <xtimer.h>
#include <stdio.h>
#include <event/thread.h>

// Function to be called periodically
void event_periodic(void *arg) {
    (void)arg; // Suppress unused argument warning
    printf("Periodic event triggered!\n");
}

// Main function
int main(void) {
    // Define a timer with a 1-second interval
    ztimer_t timer;
    timer.callback = event_periodic;
    timer.arg = NULL;
    ztimer_init(ZTIMER_SEC, &timer);

    // Set the timer to trigger every second
    ztimer_periodic(ZTIMER_SEC, &timer, ZTIMER_MSEC(1000));
    
    // Keep running the event loop to trigger periodic events
    while(1) {
        ztimer_ticks_t now = ztimer_now(ZTIMER_SEC);
        if (ztimer_period_expired(&timer, now)) {
            event_periodic(NULL);
            ztimer_add_absolute(ZTIMER_SEC, &timer, now + ZTIMER_SEC);
        }
    }

    return 0;
}