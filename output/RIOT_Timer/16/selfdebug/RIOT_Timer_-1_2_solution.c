#include <timex.h>
#include <xtimer.h>
#include <periph/gpio.h>

#define TIMER_INTERVAL (1U * US_PER_SEC) // 1 second in microseconds
#define LED_PIN GPIO_PIN(0, 2) // Example pin, change according to your setup

static void timer_callback(void *arg) {
    (void)arg;
    gpio_toggle(LED_PIN);
}

int main(void) {
    xtimer_ticks32_t last_wakeup = xtimer_now();

    // Initialize the LED pin
    gpio_init(LED_PIN, GPIO_OUT);

    while (1) {
        timer_callback(NULL);
        xtimer_periodic_wakeup(&last_wakeup, TIMER_INTERVAL);
    }

    return 0;
}