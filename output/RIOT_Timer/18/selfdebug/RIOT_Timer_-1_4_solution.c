#include "xtimer.h"
#include "timex.h"
#include "periph/gpio.h"

#define LED_PIN GPIO_PIN(PORT_A, 0)

static void timer_callback(void *arg) {
    (void)arg;
    // Toggle LED
    gpio_toggle(LED_PIN);
}

int main(void) {
    // Initialize the LED pin
    gpio_init(LED_PIN, GPIO_OUT);

    // Initialize timer
    xtimer_ticks32_t interval = { .ticks32 = XTIMER_SEC(1) };
    xtimer_t timer = { .callback = timer_callback, .arg = NULL };

    while (1) {
        xtimer_set(&timer, interval);
    }

    return 0;
}