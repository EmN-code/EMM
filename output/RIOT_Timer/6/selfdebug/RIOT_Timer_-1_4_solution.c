#include <xtimer.h> // Includes the XTIMER functions for setting up a timer.
#include <timex.h>  // Includes TIMEX for time-related utilities.
#include <thread.h> // Basic thread functionalities.
#include <stdio.h>  // Standard input/output.
#include <stdint.h> // Standard integer types.
#include <board.h>  // Include board dependent definitions, such as LED_PORT

#define INTERVAL (1U * US_PER_SEC) // 1 second interval

static void timer_callback(void *arg) {
    (void)arg;
    // Code to toggle the LED state
    LED0_ON;
    xtimer_sleep(1); // Just for visual perception, turn off after 1 second
    LED0_OFF;
}

int main(void) {
    // Initialize the board, usually sets port as output
    LED0_ON;
    xtimer_ticks32_t last_wakeup = xtimer_now();
    
    // Initialize a timer that calls our callback function every INTERVAL
    while (1) {
        xtimer_periodic_wakeup(&last_wakeup, INTERVAL);
        timer_callback(NULL);
    }

    return 0;
}