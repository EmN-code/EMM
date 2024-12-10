
#include "xtimer.h"
#include "periph/gpio.h"

#define LED_PIN GPIO_PIN(0, 2)  // Adjust this to your specific LED pin
#define TIMER_INTERVAL (1 * 1000 * 1000U)  // 1 second in microseconds

void timer_callback(void *arg) {
    (void)arg;  // Unused argument
    gpio_toggle(LED_PIN);
    xtimer_set(&timer, TIMER_INTERVAL);
}

int main(void) {
    xtimer_t timer;

    // Initialize the LED pin as output
    gpio_init(LED_PIN, GPIO_OUT);

    // Set up the timer
    timer.callback = timer_callback;
    timer.arg = NULL;

    // Start the timer
    xtimer_set(&timer, TIMER_INTERVAL);

    return 0;
}
