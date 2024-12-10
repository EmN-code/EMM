
#include "xtimer.h"
#include "periph/gpio.h"

#define LED_PIN GPIO_PIN(0, 16)  // Assuming GPIO0, pin 16 is connected to the LED

void toggle_led(void) {
    static int state = 0;
    gpio_write(LED_PIN, state);
    state = !state;
}

int main(void) {
    // Initialize the LED pin as output
    gpio_init(LED_PIN, GPIO_OUT);

    // Initialize xtimer
    xtimer_init();

    // Set up a periodic timer to trigger every second
    xtimer_t timer;
    xtimer_periodic_wakeup(&timer, XTIMER_TICKS(1 * US_PER_SEC), toggle_led);

    return 0;
}
