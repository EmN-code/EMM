
#include "xtimer.h"
#include "periph/gpio.h"

#define LED_PIN GPIO_PIN(0, 16)  // Assuming GPIO 16 on port 0 is connected to the LED

void toggle_led(void) {
    static int state = 0;
    gpio_write(LED_PIN, state);
    state = !state;
}

int main(void) {
    // Initialize the LED pin as output
    gpio_init(LED_PIN, GPIO_OUT);

    // Set the LED pin to low initially
    gpio_clear(LED_PIN);

    // Create a timer that triggers every second
    xtimer_t timer;
    xtimer_init(&timer);

    while (1) {
        // Wait for the timer to trigger
        xtimer_usleep(1000000);  // 1 second delay

        // Toggle the LED
        toggle_led();
    }

    return 0;
}