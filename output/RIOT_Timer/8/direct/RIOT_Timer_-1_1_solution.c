#include <stdio.h>
#include <periph/gpio.h>
#include <xtimer.h>

#define LED_PIN GPIO_PIN(0, 2) // Assuming GPIO2 is connected to an LED.

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

    xtimer_ticks32_t interval = xtimer_usec_from_sec(1);
    while (1) {
        toggle_led();
        xtimer_periodic_wakeup(NULL, interval);
    }
    return 0;
}