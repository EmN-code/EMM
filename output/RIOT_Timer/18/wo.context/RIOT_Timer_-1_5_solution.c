#include "xtimer.h"
#include "periph/gpio.h"

#define INTERVAL (1U * US_PER_SEC)
#define LED_PIN GPIO_PIN(0, 5) // Assuming GPIO Port 0, Pin 5

void led_toggle(void) {
    static int state = 0;
    gpio_write(LED_PIN, state);
    state = !state;
}

int main(void) {
    gpio_init(LED_PIN, GPIO_OUT);
    xtimer_ticks32_t last_wakeup = xtimer_now();
    while (1) {
        led_toggle();
        xtimer_periodic_wakeup(&last_wakeup, INTERVAL);
    }
    return 0;
}