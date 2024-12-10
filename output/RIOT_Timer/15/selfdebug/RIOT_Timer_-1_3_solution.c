#include "xtimer.h"
#include "ztimer.h"
#include "periph/gpio.h"
#include <stdio.h>

#define LED_PIN GPIO_PIN(0, 2)  // Assumed pin number; will vary depending on ESP32 board

static void timer_callback(void *arg) {
    (void)arg; // unused
    static bool led_state = false;

    if (led_state) {
        gpio_clear(LED_PIN);
    }
    else {
        gpio_set(LED_PIN);
    }
    led_state = !led_state;
}

int main(void) {
    gpio_init(LED_PIN, GPIO_OUT);

    xtimer_ticks32_t last_wakeup = xtimer_now();
    while (1) {
        xtimer_periodic_wakeup(&last_wakeup, 1000000);
        timer_callback(NULL);
    }
    return 0;
}