#include <stdio.h>
#include <periph/gpio.h>
#include <periph/timer.h>

#define TIMER_INTERVAL (1U * US_PER_SEC)

static void timer_callback(void *arg) {
    (void)arg;
    gpio_set(GPIO_PIN(PORT_A, 0));  // Assuming the LED is on port A pin 0
    xtimer_usleep(TIMER_INTERVAL);  // ensure LED is visible
    gpio_clear(GPIO_PIN(PORT_A, 0));
}

int main(void) {
    gpio_init(GPIO_PIN(PORT_A, 0), GPIO_OUT);

    timer_t timer;
    timer_init(&timer, TIMER_INTERVAL, timer_callback, NULL);

    timer_start(&timer);

    while (1) {
        // Keep the main function running
    }

    return 0;
}