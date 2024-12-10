
#include <stdio.h>
#include "xtimer.h"
#include "periph/gpio.h"

#define LED_PIN GPIO_PIN(0, 13)  // Assuming GPIO13 is connected to an LED on ESP32

void led_toggle(void) {
    gpio_toggle(LED_PIN);
}

int main(void) {
    // Initialize the LED pin as output
    gpio_init(LED_PIN, GPIO_OUT);

    // Create a timer instance
    xtimer_t timer;

    // Set the timer to trigger every second
    xtimer_init(&timer);
    xtimer_set(&timer, 1 * 1000 * 1000, led_toggle, NULL);  // 1 second in microseconds

    // Main loop to keep the application running
    while (1) {
        // The timer will handle the LED toggling
        xtimer_usleep(1 * 1000 * 1000);  // Sleep for 1 second
    }

    return 0;
}
