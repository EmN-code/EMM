#include <xtimer.h>
#include <stdio.h>
#include <periph/gpio.h>

#define LED_PIN     GPIO_PIN(0, 2)  // GPIO pin for LED, adjust if needed
#define TIMER_INTERVAL (1U * US_PER_SEC)  // 1 second interval

static void timer_callback(void *arg) {
    (void)arg;
    gpio_toggle(LED_PIN);
}

int main(void) {
    if (gpio_init(LED_PIN, GPIO_OUT)) {
        puts("Error initializing GPIO pin");
        return 1;
    }

    xtimer_ticks32_t last_wakeup = xtimer_now();

    while (1) {
        xtimer_periodic_wakeup(&last_wakeup, TIMER_INTERVAL);
        timer_callback(NULL);
    }

    return 0;
}