#include <xtimer.h> // Include the xtimer module for timer management
#include <timex.h>  // Include timex for time-related functions
#include <stdio.h>  // Include standard input/output library
#include "periph/gpio.h" // Include GPIO header for controlling GPIO pins

#define INTERVAL (1U * US_PER_SEC) // Set interval to 1 second
#define LED_PIN (GPIO_PIN(PORT_A, 5)) // Define the GPIO pin where the LED is connected, change PORT and PIN as per your board

void timer_callback(void *arg) {
    (void)arg; // Avoid unused parameter warning
    gpio_toggle(LED_PIN); // Toggle the LED state
}

int main(void) {
    // Initialize the GPIO pin as output
    if (gpio_init(LED_PIN, GPIO_OUT)) {
        printf("Error initializing LED pin\n");
        return 1;
    }

    // Initialize the timer
    xtimer_ticks32_t last_wakeup = xtimer_now();
    while (1) {
        xtimer_periodic_wakeup(&last_wakeup, INTERVAL);
        timer_callback(NULL);
    }

    return 0; // Main should return an integer
}
