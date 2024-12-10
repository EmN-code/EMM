// Description: This program sets up a timer on an ESP32 using the RIOT operating system, 
// turning on an LED every second.

// Importing necessary libraries in the required order
#include "xtimer.h"
#include "periph/gpio.h"
#include <stdio.h>

int main(void) {
    // Initialize the GPIO pin for the LED
    gpio_t led_pin = GPIO_PIN(0, 2); // Depending on your board, adjust the port and pin number
    gpio_init(led_pin, GPIO_OUT);

    // Setting up the variables
    xtimer_ticks32_t last_wakeup = xtimer_now();

    // Main program loop
    while (1) {
        // Turn on the LED
        gpio_set(led_pin);

        // Wait for one second
        xtimer_periodic_wakeup(&last_wakeup, 1 * 1000000U);

        // Turn off the LED
        gpio_clear(led_pin);

        // Wait for another second if needed or do something else
    }

    return 0;
}