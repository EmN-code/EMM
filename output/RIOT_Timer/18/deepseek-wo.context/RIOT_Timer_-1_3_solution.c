
#include "xtimer.h"
#include "periph/gpio.h"

#define LED_PIN GPIO_PIN(0, 2)  // Assuming GPIO0, pin 2 is connected to the LED

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
    xtimer_init(&timer);
    xtimer_set_periodic(&timer, XTIMER_TICKS(1 * US_PER_SEC), toggle_led);

    // Keep the main thread alive
    while (1) {
        xtimer_usleep(1000000);  // Sleep for 1 second
    }

    return 0;
}
