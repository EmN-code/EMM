#include <xtimer.h>
#include <periph/gpio.h>

#define LED_PIN GPIO_PIN(PORT_B, 5)
#define TIMER_INTERVAL 1000000 // 1 second in microseconds

void timer_callback(void *arg)
{
    (void)arg;
    gpio_toggle(LED_PIN);
}

int main(void)
{
    xtimer_ticks32_t last_wakeup = xtimer_now();
    gpio_init(LED_PIN, GPIO_OUT);

    while (1) {
        xtimer_periodic_wakeup(&last_wakeup, TIMER_INTERVAL);
        timer_callback(NULL);
    }

    return 0;
}
