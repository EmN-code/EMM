
#include <stdio.h>
#include "xtimer.h"
#include "periph/gpio.h"

#define LED_PIN GPIO_PIN(0, 5)  // Assuming GPIO5 on ESP32

void led_toggle(void) {
    gpio_toggle(LED_PIN);
}

int main(void) {
    // Initialize the LED pin as output
    gpio_init(LED_PIN, GPIO_OUT);

    // Create a timer that triggers every second
    xtimer_t timer;
    uint32_t period = 1000000;  // 1 second in microseconds

    // Start the timer
    xtimer_init(&timer);
    xtimer_set(&timer, period, (xtimer_callback_t)led_toggle, NULL);

    // Main loop to keep the application running
    while (1) {
        // The timer will handle the LED toggling
        xtimer_usleep(period);
    }

    return 0;
}
