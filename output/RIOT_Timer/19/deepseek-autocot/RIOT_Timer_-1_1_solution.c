
#include <stdio.h>
#include "xtimer.h"
#include "periph/gpio.h"

#define LED_PIN GPIO_PIN(0, 13)  // Assuming GPIO 13 is connected to an LED on ESP32

void led_toggle(void) {
    gpio_toggle(LED_PIN);
}

int main(void) {
    // Initialize the LED pin as output
    gpio_init(LED_PIN, GPIO_OUT);

    // Create a timer that triggers every second
    xtimer_t timer;
    uint32_t period = 1 * 1000 * 1000;  // 1 second in microseconds

    // Initialize the timer
    xtimer_init(&timer);

    // Set the timer to trigger the led_toggle function every second
    xtimer_set(&timer, period, (xtimer_callback_t)led_toggle, NULL);

    // Main loop to keep the program running
    while (1) {
        // Sleep to prevent busy-waiting
        xtimer_usleep(1000000);  // Sleep for 1 second
    }

    return 0;
}
