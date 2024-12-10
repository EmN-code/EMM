#include <xtimer.h>
#include <periph/gpio.h>

#define LED_PIN GPIO_PIN(0, 2) // Use GPIO2 for LED
#define TIMER_INTERVAL (1U * US_PER_SEC) // 1 second interval

static void led_toggle(void *arg) {
    (void)arg;
    gpio_toggle(LED_PIN);
}

int main(void) {
    // Initialize the GPIO pin
    gpio_init(LED_PIN, GPIO_OUT);

    // Set up the timer
    xtimer_ticks32_t last_wakeup = xtimer_now();

    while (1) {
        led_toggle(NULL);
        xtimer_periodic_wakeup(&last_wakeup, TIMER_INTERVAL);
    }

    return 0;
}