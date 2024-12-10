#include <stdio.h>
#include "xtimer.h"
#include "periph/gpio.h"

#define LED_PIN GPIO_PIN(PORT_A, 5) // Assuming this pin is connected to the LED

void timer_callback(void *arg) {
    (void)arg;
    gpio_toggle(LED_PIN);
    printf("LED toggled!\n");
}

int main(void) {
    // Initialize the LED pin
    gpio_init(LED_PIN, GPIO_OUT);

    // Timer configuration
    xtimer_ticks32_t last = xtimer_now();
    while (1) {
        xtimer_periodic_wakeup(&last, xtimer_usec_to_ticks(1 * US_PER_SEC));
        timer_callback(NULL);
    }

    return 0;
}