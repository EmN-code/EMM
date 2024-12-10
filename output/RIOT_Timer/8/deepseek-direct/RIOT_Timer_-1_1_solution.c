
#include "xtimer.h"
#include "periph/gpio.h"

#define LED_PIN GPIO_PIN(0, 16)  // Assuming GPIO 16 on ESP32

void toggle_led(void) {
    static int state = 0;
    gpio_write(LED_PIN, state);
    state = !state;
}

int main(void) {
    // Initialize the LED pin as output
    gpio_init(LED_PIN, GPIO_OUT);

    // Create a timer that triggers every second
    xtimer_t timer;
    uint32_t interval = 1000000;  // 1 second in microseconds

    // Initialize the timer
    xtimer_init(&timer);

    while (1) {
        // Wait for the timer to trigger
        xtimer_usleep(interval);

        // Toggle the LED
        toggle_led();
    }

    return 0;
}
