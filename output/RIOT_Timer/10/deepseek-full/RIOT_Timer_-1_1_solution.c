
#include <stdio.h>
#include "xtimer.h"
#include "periph/gpio.h"

#define LED_PIN GPIO_PIN(0, 13)  // Assuming GPIO 13 is connected to an LED on ESP32

void timer_callback(void *arg) {
    (void)arg;  // Unused argument
    gpio_toggle(LED_PIN);  // Toggle the LED state
}

int main(void) {
    // Initialize the LED pin as output
    gpio_init(LED_PIN, GPIO_OUT);

    // Create a timer that triggers every second
    xtimer_t timer;
    uint32_t interval = 1000000;  // 1 second in microseconds

    // Initialize the timer
    xtimer_init(&timer);

    // Set the timer to trigger the callback every second
    xtimer_set(&timer, interval, timer_callback, NULL);

    // Main loop to keep the program running
    while (1) {
        // The timer will handle the LED toggling in the background
        xtimer_usleep(interval);  // Sleep for the same interval to avoid busy-waiting
    }

    return 0;
}