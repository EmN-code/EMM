
#include "xtimer.h"
#include "periph/gpio.h"

#define LED_PIN GPIO_PIN(0, 16)  // Assuming GPIO 16 on port 0 is connected to an LED

static xtimer_t timer;
static uint32_t interval = 1000000;  // 1 second in microseconds

void toggle_led(void) {
    gpio_toggle(LED_PIN);
}

int main(void) {
    // Initialize GPIO for the LED
    gpio_init(LED_PIN, GPIO_OUT);

    // Initialize xtimer (no arguments needed)
    xtimer_init();

    // Set up the timer to trigger every second
    xtimer_set(&timer, interval, toggle_led, NULL);

    // Keep the main thread alive
    while (1) {
        xtimer_usleep(interval);
    }

    return 0;
}
