
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

    // Create a timer instance
    xtimer_t timer;
    uint32_t interval = 1000000;  // 1 second in microseconds

    // Set the timer to trigger every second
    while (1) {
        xtimer_usleep(interval);
        led_toggle();
    }

    return 0;
}
