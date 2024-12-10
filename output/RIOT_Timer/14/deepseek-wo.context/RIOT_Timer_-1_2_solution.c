
#include "xtimer.h"
#include "periph/gpio.h"

#define LED_PIN GPIO_PIN(0, 2)  // Assuming GPIO0, pin 2 is connected to the LED
#define TIMER_INTERVAL (1 * 1000 * 1000U)  // 1 second in microseconds

void timer_callback(void *arg) {
    (void)arg;  // Unused argument
    gpio_toggle(LED_PIN);  // Toggle the LED state
}

int main(void) {
    // Initialize the LED pin as output
    gpio_init(LED_PIN, GPIO_OUT);

    // Initialize the xtimer (no arguments needed for xtimer_init)
    xtimer_init();

    // Set up the timer to trigger every second
    xtimer_t timer;
    timer.callback = timer_callback;
    timer.arg = NULL;

    // Start the timer (no arguments needed for xtimer_set)
    xtimer_set(&timer, TIMER_INTERVAL);

    return 0;
}
