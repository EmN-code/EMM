// This code initializes an ESP32 in the RIOT OS environment, sets up a repeating timer, and toggles an LED connected to a GPIO pin every second.

// Import necessary RIOT OS headers
#include "xtimer.h"
#include "periph/gpio.h"

// Description of the solution:
// This program utilizes RIOT OS's xtimer for scheduling periodic tasks. It sets up a timer that triggers every second, interacting with ESP32's GPIO to toggle an LED.

#define INTERVAL (1U * US_PER_SEC) // Defines the timer interval to be 1 second
#define LED_PIN GPIO_PIN(PORT_B, 0) // Assuming the LED is connected to pin PORT_B, 0

void toggle_led(void) {
    static bool led_on = false; // Tracks LED state
    
    if (led_on) {
        gpio_clear(LED_PIN);
    } else {
        gpio_set(LED_PIN);
    }

    led_on = !led_on; // Toggle the LED state
}

int main(void) {
    // Initialize the GPIO pin for output
    if (gpio_init(LED_PIN, GPIO_OUT)) {
        return -1; // Error initializing GPIO
    }

    // Initialize the timer
    xtimer_ticks32_t last_wakeup = xtimer_now();
    while (1) {
        toggle_led(); // Toggle LED state
        xtimer_periodic_wakeup(&last_wakeup, INTERVAL); // Wait for next interval
    }

    return 0; // While loop is infinite, return will not be reached
}