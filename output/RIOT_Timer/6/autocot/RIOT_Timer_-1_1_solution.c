#include <timex.h>
#include <stdio.h>
#include <periph/gpio.h>
#include <xtimer.h>

#define LED_PIN GPIO_PIN(PORT_A, 2)
#define INTERVAL (1U * US_PER_SEC) // 1 second

static void timer_callback(void *arg) {
    (void)arg;
    gpio_toggle(LED_PIN); // Toggle the LED state
}

int main(void) {
    // Initialize the LED pin
    gpio_init(LED_PIN, GPIO_OUT);

    // Initialize the timer with the specified interval and callback
    xtimer_ticks32_t interval = xtimer_usec_to_ticks32(INTERVAL);
    xtimer_t timer;
    timer.callback = timer_callback;
    timer.arg = NULL;

    while (1) {
        // Set the timer with a periodic interval
        xtimer_set(&timer, interval);
        xtimer_sleep(1); // Sleep to allow the timer to process
    }

    return 0;
}