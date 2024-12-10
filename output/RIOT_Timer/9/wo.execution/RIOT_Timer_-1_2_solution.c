#include "xtimer.h"
#include <stdio.h>

// Define the timer interval (1 second)
#define TIMER_INTERVAL (1U * 1000000U) // in microseconds

// GPIO pin for the LED, assuming GPIO_PIN(0, 2) for demonstration
#define LED_PIN GPIO_PIN(0, 2)

// Timer and pin state
static xtimer_t timer;

// Callback function for the timer
void timer_callback(void *arg) {
    (void)arg;
    // Toggle LED state
    gpio_toggle(LED_PIN);
    // Reset the timer
    xtimer_set(&timer, TIMER_INTERVAL);
}

int main(void) {
    // Initialize the GPIO for the LED
    gpio_init(LED_PIN, GPIO_OUT);

    // Initialize the timer
    xtimer_init();
    timer.callback = timer_callback;

    // Set the timer for the first time
    xtimer_set(&timer, TIMER_INTERVAL);

    // Loop indefinitely
    while (1) {
        // Placeholder for other code
        // Put the MCU to sleep to save power
        xtimer_usleep(1000000);
    }

    return 0;
}
