#include <xtimer.h>
#include <periph/gpio.h>

#define LED_PIN GPIO_PIN(0, 2) // Change to your LED pin if needed

void toggle_led(void) {
    static int led_state = 0;
    if (led_state) {
        gpio_clear(LED_PIN);
    } else {
        gpio_set(LED_PIN);
    }
    led_state = !led_state;
}

int main(void) {
    gpio_init(LED_PIN, GPIO_OUT);
    xtimer_ticks32_t last_wakeup = xtimer_now();

    while (1) {
        toggle_led();
        xtimer_periodic_wakeup(&last_wakeup, 1U * US_PER_SEC);
    }

    return 0;
}