#include <stdio.h>
#include "xtimer.h"
#include "periph/gpio.h"

#define INTERVAL (1000000U) // Timer interval in microseconds
#define LED_PIN GPIO_PIN(0, 2) // Define which GPIO pin the LED is connected to

// Function to be called on each timer trigger
void timer_callback(void *arg) {
    (void)arg; // Unused
    static bool led_on = false;
    led_on = !led_on; // Toggle LED state
    if (led_on) {
        gpio_set(LED_PIN);
        printf("LED is ON\n");
    } else {
        gpio_clear(LED_PIN);
        printf("LED is OFF\n");
    }
}

// Main function
int main(void) {
    // Initialize the GPIO pin connected to the LED
    if (gpio_init(LED_PIN, GPIO_OUT)) {
        printf("Error initializing GPIO\n");
        return 1;
    }

    // Print start message
    printf("Starting the timer application\n");

    // Initialize the xtimer
    xtimer_ticks32_t last_wakeup = xtimer_now();

    // Run the timer loop
    while (1) {
        xtimer_periodic_wakeup(&last_wakeup, INTERVAL);
        timer_callback(NULL); // Call the timer callback
    }

    return 0;
}