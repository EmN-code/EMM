
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

    // Create a timer that triggers every second
    xtimer_t timer;
    uint32_t period = 1000000;  // 1 second in microseconds

    // Initialize and start the timer
    xtimer_init(&timer);
    xtimer_set_periodic(&timer, period, timer_callback, NULL);

    // Main loop to keep the application running
    while (1) {
        // The timer callback will handle the LED toggling
        xtimer_usleep(period);  // Sleep for the timer period
    }

    return 0;
}
