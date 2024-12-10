
#include <stdio.h>
#include "xtimer.h"
#include "periph/gpio.h"
#include "event/periodic_callback.h"

#define LED_PIN GPIO_PIN(0, 13)  // Assuming GPIO 13 is connected to an LED on ESP32
#define TIMER_INTERVAL (1 * 1000 * 1000U)  // 1 second in microseconds

static void led_toggle(void) {
    gpio_toggle(LED_PIN);
}

int main(void) {
    // Initialize the LED pin as output
    gpio_init(LED_PIN, GPIO_OUT);

    // Create a periodic callback that triggers every second
    periodic_callback_t callback;
    periodic_callback_init(&callback, TIMER_INTERVAL, led_toggle);

    // Start the periodic callback
    periodic_callback_start(&callback);

    // Keep the main thread alive
    while (1) {
        xtimer_sleep(1);
    }

    return 0;
}
