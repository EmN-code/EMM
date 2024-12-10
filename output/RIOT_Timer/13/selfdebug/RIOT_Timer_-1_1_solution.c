#include <timers.h>
#include <periph/gpio.h>
#include <xtimer.h>

#define LED_PIN GPIO_PIN(PORT_A, 0) // Example pin
#define TIMER_INTERVAL (1U * US_PER_SEC) // 1 second interval

static void _timer_callback(void *arg)
{
    (void)arg;
    gpio_toggle(LED_PIN);
}

int main(void)
{
    // Initialize GPIO for the LED
    gpio_init(LED_PIN, GPIO_OUT);

    // Set up a periodic timer using xtimer
    xtimer_ticks32_t last_wakeup = xtimer_now();
    while (1) {
        xtimer_periodic_wakeup(&last_wakeup, TIMER_INTERVAL);
        _timer_callback(NULL);
    }

    return 0;
}