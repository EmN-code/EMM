
#include "xtimer.h"
#include "periph/gpio.h"

#define LED_PIN GPIO_PIN(0, 2)  // Assuming GPIO0, pin 2 is connected to the LED

void led_toggle(void) {
    gpio_toggle(LED_PIN);
}

int main(void) {
    // Initialize the LED pin as output
    gpio_init(LED_PIN, GPIO_OUT);

    // Create a periodic timer
    xtimer_t timer;
    xtimer_init(&timer);  // Initialize the timer

    // Set the timer to trigger every second
    xtimer_set_periodic(&timer, XTIMER_TICKS(1000000), (void (*)(void *))led_toggle, NULL);

    // Keep the main thread alive
    while (1) {
        xtimer_sleep(1);
    }

    return 0;
}
