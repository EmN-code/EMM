// This code sets up a timer on an ESP32 using RIOT OS.
// The timer toggles an LED every second.

#include "periph/gpio.h"
#include "timex.h"
#include "timex.h"  // include timex.h for timer functions

void callback(void *arg) {
    // Define the pin where the LED is connected
    static gpio_t led_pin = GPIO_PIN(0, 2);
    static int led_status = 0;

    // Toggle the LED status
    led_status = !led_status;
    gpio_write(led_pin, led_status);
}

int main(void) {
    // Initialize the LED pin
    gpio_init(GPIO_PIN(0, 2), GPIO_OUT);

    // Set the timer variables
    tim_t timer;
    uint32_t freq = 1 * US_PER_SEC;

    // Initialize the timer
    timer_init(timer, freq, callback, NULL);

    // Keep the main alive
    while (1) {
        // We rely on the timer interrupt to toggle the LED
    }

    return 0;
}