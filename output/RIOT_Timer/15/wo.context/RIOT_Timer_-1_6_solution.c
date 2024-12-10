// Include necessary headers in descending order by their first letter.
#include <stdio.h>
#include <periph/gpio.h>
#include <xtimer.h>  // for sleep and timer setup
#include "periph_conf.h"

// Description: This code sets up a periodic timer on an ESP32 that triggers every second. When triggered, it turns on an LED connected to a specific GPIO pin. 
// It handles potential issues with the xtimer_periodic_wakeup API, ensuring correct argument passing and return value handling.

#define LED_PIN GPIO_PIN(0, 2)  // Define GPIO pin where LED is connected

void init_led(void) {
    // Initialize the GPIO pin as output
    gpio_init(LED_PIN, GPIO_OUT);
}

void toggle_led(void) {
    // Toggle LED state
    gpio_toggle(LED_PIN);
}

int main(void) {
    init_led();  // Initialize the LED pin
    uint32_t last_wakeup = xtimer_now();  // Use xtimer_now to get current time as a 32-bit integer

    while (1) {
        last_wakeup = xtimer_now();  // Update last_wakeup to current time every loop

        // Turn the LED on
        gpio_set(LED_PIN);

        // Wait for one second using xtimer_periodic_wakeup
        xtimer_periodic_wakeup(&last_wakeup, xtimer_usec_from_sec(1));

        // Optionally turn off LED after some operations
        gpio_clear(LED_PIN);  // Clear pin to turn off LED
    }

    return 0;
}