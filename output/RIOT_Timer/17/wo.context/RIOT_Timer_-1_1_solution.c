#include "xtimer.h"
#include "periph/gpio.h"

#define LED_PIN GPIO_PIN(0, 2)  // Assuming GPIO 2 is used
#define INTERVAL (1U * XTIMER_SEC)  // 1 second interval

static void timer_callback(void *arg) {
    (void)arg;
    // Toggle LED state
    if (gpio_read(LED_PIN)) {
        gpio_clear(LED_PIN);
    } else {
        gpio_set(LED_PIN);
    }
}

int main(void) {
    // Initialize LED pin
    gpio_init(LED_PIN, GPIO_OUT);

    // Configure the timer
    xtimer_t timer;
    timer.callback = timer_callback;
    timer.arg = NULL;
    timer.target.ticks32 = xtimer_now().ticks32 + INTERVAL;

    // Start the timer
    xtimer_set(&timer, INTERVAL);

    // Let the thread sleep while the timer operates
    while (1) {
        thread_yield(); // or xtimer_sleep when required
    }

    return 0;
}