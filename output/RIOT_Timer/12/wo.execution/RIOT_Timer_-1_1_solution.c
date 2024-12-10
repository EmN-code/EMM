#include "timer.h"
#include "periph/gpio.h"

#define LED_PIN     GPIO_PIN(0, 2)
#define TIMER_DEV   (TIMER_DEV(0))

static void timer_callback(void *arg)
{
    (void)arg;
    gpio_toggle(LED_PIN);
}

int main(void)
{
    gpio_init(LED_PIN, GPIO_OUT);

    timer_init(TIMER_DEV, 1, timer_callback, NULL);
    timer_start(TIMER_DEV);

    while (1) {
        /* Keep the main thread running to let the timer operate */
    }

    return 0;
}