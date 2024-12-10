
#include <stdio.h>
#include "xtimer.h"
#include "periph/gpio.h"

#define LED_PIN GPIO_PIN(0, 16)  // Example GPIO pin for LED on ESP32

void led_toggle(void) {
    gpio_toggle(LED_PIN);
}

int main(void) {
    // Initialize the LED pin as output
    gpio_init(LED_PIN, GPIO_OUT);

    // Create a timer that triggers every second
    xtimer_t timer;
    uint32_t interval = 1 * 1000 * 1000;  // 1 second in microseconds

    while (1) {
        // Toggle the LED
        led_toggle();

        // Sleep for the specified interval
        xtimer_usleep(interval);
    }

    return 0;
}
