#include <xtimer.h>
#include <stdio.h>
#include <periph/gpio.h>

#define LED_PIN GPIO_PIN(0, 2)  // Assumes GPIO 2 for LED, adjust as per your setup

void toggle_led(void)
{
    static int led_state = 0;
    led_state = !led_state;
    if (led_state) {
        gpio_set(LED_PIN);
    } else {
        gpio_clear(LED_PIN);
    }
}

int main(void)
{
    gpio_init(LED_PIN, GPIO_OUT);
    xtimer_ticks32_t last_wakeup = xtimer_now();
    while (1) {
        toggle_led();
        xtimer_periodic_wakeup(&last_wakeup, XTIMER_SEC(1));
    }
    return 0;
}