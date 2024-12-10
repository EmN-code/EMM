
#include "xtimer.h"
#include "periph/gpio.h"

#define LED_PIN GPIO_PIN(0, 16)  // Assuming GPIO 16 on ESP32 is connected to an LED

void toggle_led(void) {
    gpio_toggle(LED_PIN);
}

int main(void) {
    // Initialize the LED pin as output
    gpio_init(LED_PIN, GPIO_OUT);

    // Create a timer that triggers every second
    xtimer_t timer;
    xtimer_init(&timer);

    while (1) {
        // Toggle the LED
        toggle_led();

        // Sleep for 1 second
        xtimer_usleep(1000 * 1000);
    }

    return 0;
}
