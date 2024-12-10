#include <xtimer.h>  // Include xtimer for timer functions
#include <stdio.h>
#include <periph/gpio.h>  // Include GPIO peripheral to control I/O pins

#define LED_PIN GPIO_PIN(0, 2)  // Define the GPIO pin for the LED, assuming ESP32 and LED is on GPIO pin 2

// Function to initialize the LED pin
void init_led(void) {
    gpio_init(LED_PIN, GPIO_OUT);
}

// Main function to control the flow
int main(void) {
    // Initialize the LED pin
    init_led();

    // Timer loop
    while (1) {
        gpio_set(LED_PIN);  // Turn on the LED
        xtimer_sleep(1);  // Sleep for 1 second
        gpio_clear(LED_PIN);  // Turn off the LED
        xtimer_sleep(1);  // Sleep for 1 second
    }
    return 0;  // Typically not reached in an embedded application
}