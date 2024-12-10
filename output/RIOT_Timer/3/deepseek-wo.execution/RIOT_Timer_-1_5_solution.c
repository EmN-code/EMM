
#include <stdio.h>
#include "xtimer.h"
#include "periph/gpio.h"

#define LED_PIN GPIO_PIN(0, 16)  // Example GPIO pin for ESP32
#define TIMER_INTERVAL (1 * 1000 * 1000U)  // 1 second in microseconds

static xtimer_t timer;

void timer_callback(void *arg) {
    (void)arg;
    gpio_toggle(LED_PIN);  // Toggle the LED
    xtimer_set(&timer, TIMER_INTERVAL);  // Reset the timer
}

int main(void) {
    // Initialize the LED pin as output
    gpio_init(LED_PIN, GPIO_OUT);

    // Set up the timer to trigger the callback every second
    xtimer_set(&timer, TIMER_INTERVAL);

    // Start the timer
    timer.callback = timer_callback;

    // Main loop (not strictly necessary for this example, but kept for completeness)
    while (1) {
        xtimer_usleep(1000000);  // Sleep for 1 second
    }

    return 0;
}
