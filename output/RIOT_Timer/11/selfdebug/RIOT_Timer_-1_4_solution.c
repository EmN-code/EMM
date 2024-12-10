#include <stdio.h>
#include <xtimer.h>
#include <periph/gpio.h>

#define LED_PIN GPIO_PIN(PORT_A, 0)

static void timer_callback(void *arg)
{
    (void)arg;
    gpio_set(LED_PIN);
}

int main(void)
{
    gpio_init(LED_PIN, GPIO_OUT);
    xtimer_ticks32_t last_wakeup = xtimer_now();

    while (1) {
        timer_callback(NULL);
        xtimer_periodic_wakeup(&last_wakeup, US_PER_SEC);
        gpio_clear(LED_PIN);
    }

    return 0;
}