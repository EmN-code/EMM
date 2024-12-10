#include <periph/gpio.h>
#include <xtimer.h>

#define INTERVAL (1U * US_PER_SEC)
#define LED_PIN GPIO_PIN(PORT_A, 10)

static void toggle_led(void *arg)
{
    (void)arg;
    gpio_toggle(LED_PIN);
}

int main(void)
{
    /* initialize the LED pin */
    gpio_init(LED_PIN, GPIO_OUT);

    /* create a timer */
    xtimer_ticks32_t last_wakeup = xtimer_now();

    while (1) {
        /* wait for the next scheduled wakeup */
        xtimer_periodic_wakeup(&last_wakeup, INTERVAL);
        /* toggle the LED */
        toggle_led(NULL);
    }

    return 0;
}