#include <xtimer.h>
#include <stdio.h>
#include <periph/gpio.h>

#define TIMER_INTERVAL (1U * US_PER_SEC) // 1 second in microseconds
#define LED_PIN GPIO_PIN(0, 2) // Example pin, change to your specific ESP32 setup

static xtimer_t timer;

void timer_callback(void *arg) {
    (void)arg;
    // Toggle the LED
    gpio_toggle(LED_PIN);
    // Reset the timer
    xtimer_set(&timer, TIMER_INTERVAL);
}

int main(void) {
    // Initialize the LED pin
    gpio_init(LED_PIN, GPIO_OUT);

    // Initialize the timer
    timer.callback = timer_callback;
    timer.arg = NULL;
    xtimer_set(&timer, TIMER_INTERVAL);

    // Keep main thread alive
    while (1) {
        xtimer_sleep(1);
    }
    return 0;
}