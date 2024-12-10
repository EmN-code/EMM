
#include <stdio.h>
#include "xtimer.h"
#include "periph/gpio.h"

#define LED_PIN GPIO_PIN(0, 16)  // Example LED pin for ESP32
#define TIMER_INTERVAL (1 * 1000 * 1000U)  // 1 second in microseconds

void timer_callback(void *arg) {
    (void)arg;
    gpio_toggle(LED_PIN);  // Toggle the LED
}

int main(void) {
    // Initialize the LED pin as output
    gpio_init(LED_PIN, GPIO_OUT);

    // Create a timer
    xtimer_t timer;
    xtimer_init(&timer);

    // Set the timer to trigger every second
    xtimer_set(&timer, TIMER_INTERVAL, timer_callback, NULL);

    // Keep the main thread alive
    while (1) {
        xtimer_usleep(1000000);  // Sleep for 1 second
    }

    return 0;
}
