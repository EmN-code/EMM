// Description:
// The code sets up a timer using the xtimer module which triggers every second. This timer is used to toggle an LED on the ESP32 board. 
// The timer functionality is provided by the xtimer library in RIOT OS which allows setting callbacks on timer expiration.

// Required imports:
#include "xtimer.h"
#include "periph/gpio.h"

// Global variables setup:
#define TIMER_INTERVAL (1U * US_PER_SEC) // Interval for the timer in microseconds

gpio_t led_pin = GPIO_PIN(0, 2); // Define the GPIO pin corresponding to the LED
static xtimer_t timer; // Timer variable

// Callback function which gets called when the timer expires
void timer_callback(void *arg) {
    (void)arg; // Avoid unused parameter warning
    gpio_toggle(led_pin); // Toggle the LED state
}

int main(void) {
    // Initialize the GPIO pin as output for controlling the LED
    gpio_init(led_pin, GPIO_OUT);

    // Set up the timer structure
    timer.callback = timer_callback;
    timer.arg = NULL;

    // Arm the timer to expire every second
    while (1) {
        xtimer_set(&timer, TIMER_INTERVAL);
        xtimer_usleep(TIMER_INTERVAL);
    }

    return 0;
}