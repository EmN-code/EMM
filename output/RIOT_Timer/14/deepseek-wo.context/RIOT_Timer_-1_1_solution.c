
#include <stdio.h>
#include "xtimer.h"
#include "periph/gpio.h"

#define LED_PIN GPIO_PIN(0, 5)  // Assuming GPIO5 on ESP32

void timer_callback(void *arg) {
    (void)arg;  // Unused argument
    gpio_toggle(LED_PIN);  // Toggle the LED
}

int main(void) {
    // Initialize the LED pin as output
    gpio_init(LED_PIN, GPIO_OUT);

    // Create a timer
    xtimer_t timer;
    uint32_t period = 1000000;  // 1 second in microseconds

    // Initialize the timer
    xtimer_init(&timer);

    // Set the timer to trigger every second
    xtimer_set(&timer, period, timer_callback, NULL);

    // Main loop (keep the program running)
    while (1) {
        xtimer_usleep(1000000);  // Sleep for 1 second
    }

    return 0;
}
