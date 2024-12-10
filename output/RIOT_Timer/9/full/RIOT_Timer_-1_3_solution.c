#include "ztimer.h"
#include "xtimer.h"
#include "time_units.h"
#include "timex.h"
#include "mbox.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"

#define TIMER_INTERVAL (1U * US_PER_SEC)  // Timer interval to 1 second in microseconds

// Simulating LED state for demonstration
static bool led_state = false;

// Function to toggle LED state
void toggle_led(void) {
    led_state = !led_state;
    printf("LED is now %s\n", led_state ? "ON" : "OFF");
}

// Timer callback function
static void timer_callback(void *arg) {
    toggle_led();
}

int main(void) {
    // Initialize timer
    printf("Starting timer example...\n");
    ztimer_t timer;
    ztimer_set_wakeup(&timer, TIMER_INTERVAL, timer_callback, NULL);

    // Start the timer
    ztimer_start_periodic(&timer, TIMER_INTERVAL);

    // Keeping the main process alive to see timer effect
    while (1) {
        ztimer_sleep(ZTIMER_USEC, TIMER_INTERVAL);
    }

    return 0;
}
