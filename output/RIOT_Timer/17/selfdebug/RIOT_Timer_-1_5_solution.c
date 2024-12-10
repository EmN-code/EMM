#include <xtimer.h>
#include <stdio.h>
#include <periph/gpio.h>

#define LED_PIN GPIO_PIN(PORT_A, 0) // Assumed LED is connected to pin A0
#define INTERVAL (1U * US_PER_SEC) // 1 second interval

void toggle_led(void) {
    static int led_state = 0;
    led_state = !led_state;
    gpio_write(LED_PIN, led_state);
}

int main(void) {
    gpio_init(LED_PIN, GPIO_OUT);
    xtimer_ticks32_t last_wakeup = xtimer_now();
    while (1) {
        toggle_led();
        xtimer_periodic_wakeup(&last_wakeup, INTERVAL);
    }
    return 0;
}