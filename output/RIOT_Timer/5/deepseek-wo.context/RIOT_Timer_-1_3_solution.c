
#include "xtimer.h"
#include "periph/gpio.h"

#define LED_PIN GPIO_PIN(0, 16)  // Assuming GPIO0, pin 16 is connected to an LED

static xtimer_t timer;
static uint32_t last_wakeup = 0;

void led_toggle(void) {
    gpio_toggle(LED_PIN);
}

int main(void) {
    // Initialize the LED pin as output
    gpio_init(LED_PIN, GPIO_OUT);

    // Initialize the timer
    xtimer_init();

    // Set the timer to trigger every second
    while (1) {
        xtimer_periodic_wakeup(&last_wakeup, 1000000);  // 1 second in microseconds
        led_toggle();
    }

    return 0;
}
