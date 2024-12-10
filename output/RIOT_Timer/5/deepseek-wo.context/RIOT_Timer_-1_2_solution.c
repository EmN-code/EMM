
#include "xtimer.h"
#include "periph/gpio.h"

#define LED_PIN GPIO_PIN(0, 2)  // Assuming GPIO0, pin 2 is connected to the LED

void led_toggle(void) {
    gpio_toggle(LED_PIN);
}

int main(void) {
    // Initialize the LED pin as output
    gpio_init(LED_PIN, GPIO_OUT);

    // Initialize the xtimer
    xtimer_init();

    // Set up a periodic timer to trigger every second
    xtimer_t timer;
    xtimer_periodic_wakeup(&timer, XTIMER_TICKS(1000000));  // 1 second in microseconds

    while (1) {
        led_toggle();
        xtimer_periodic_wakeup(&timer, XTIMER_TICKS(1000000));  // 1 second in microseconds
    }

    return 0;
}
