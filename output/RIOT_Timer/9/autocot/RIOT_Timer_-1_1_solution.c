#include "timer.h"
#include "periph/gpio.h"
#include "xtimer.h"

#define LED_PIN GPIO_PIN(PORT_A, 10)

static void _timer_callback(void *arg)
{
    (void)arg;
    gpio_toggle(LED_PIN); // Toggle the LED state
}

int main(void)
{
    // Initialize the GPIO pin as output
    gpio_init(LED_PIN, GPIO_OUT);

    // Set up a timer interrupt every second (1,000,000 microseconds)
    struct xtimer_timer timer;
    xtimer_init_timer(&timer, _timer_callback, NULL);
    xtimer_set_oneshot(&timer, XTIMER_MSEC(1000));

    // Keep running
    while (1) {
        xtimer_sleep(1); // Sleep for 1 second, allowing the timer to trigger
    }

    return 0;
}