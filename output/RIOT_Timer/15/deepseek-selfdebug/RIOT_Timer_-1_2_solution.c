
#include <stdio.h>
#include "xtimer.h"
#include "periph/gpio.h"

#define LED_PIN GPIO_PIN(0, 16)  // Assuming GPIO 16 on port 0 is connected to an LED

static xtimer_t timer;
static uint32_t interval = 1000000;  // 1 second in microseconds

void toggle_led(void) {
    gpio_toggle(LED_PIN);
}

int main(void) {
    // Initialize the LED pin as output
    gpio_init(LED_PIN, GPIO_OUT);

    // Set up the timer to trigger every second
    xtimer_init(&timer);
    xtimer_set(&timer, interval);

    // Register the callback function to be called when the timer expires
    xtimer_set_cb(&timer, toggle_led);

    // Main loop to keep the program running
    while (1) {
        xtimer_usleep(interval);  // Sleep for the interval to avoid busy-waiting
    }

    return 0;
}
