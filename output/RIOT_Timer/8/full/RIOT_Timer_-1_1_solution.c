#include "xtimer.h"
#include "periph/gpio.h"

#define TIMER_INTERVAL (1U * US_PER_SEC) // 1 second
#define LED_PIN        GPIO_PIN(0, 2)   // Assuming GPIO_PIN(0, 2) is connected to the LED

static void _timer_cb(void *arg)
{
    (void)arg;
    gpio_toggle(LED_PIN);
    // Re-arm the timer
    xtimer_t *timer = (xtimer_t *)arg;
    xtimer_set(timer, TIMER_INTERVAL);
}

int main(void)
{
    // Initialize the LED pin
    gpio_init(LED_PIN, GPIO_OUT);
    
    // Initialize and start the timer
    xtimer_t timer;
    timer.callback = _timer_cb;
    timer.arg = &timer;
    timer.target = 0;
    timer.long_target = false;

    xtimer_set(&timer, TIMER_INTERVAL);
    
    while (1) {
        // Main loop does nothing, timer callback toggles the LED
        xtimer_usleep(1000000);
    }

    return 0;
}