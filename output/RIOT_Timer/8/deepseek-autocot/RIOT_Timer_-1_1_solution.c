
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
    uint32_t period = 1000000;  // 1 second in microseconds

    // Initialize the timer
    xtimer_init(&timer);

    // Set the timer to trigger the callback every second
    xtimer_set(&timer, period, timer_callback, NULL);

    // Main loop to keep the program running
    while (1) {
        // Sleep to save power, the timer will wake us up
        xtimer_usleep(period);
    }

    return 0;
}
