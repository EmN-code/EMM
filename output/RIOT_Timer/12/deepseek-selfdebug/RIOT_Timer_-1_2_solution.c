
#include "xtimer.h"
#include "periph/gpio.h"

#define LED_PIN GPIO_PIN(0, 2)  // Assuming GPIO0, pin 2 is connected to the LED
#define TIMER_INTERVAL (1 * 1000 * 1000U)  // 1 second in microseconds

static xtimer_t timer;

void timer_callback(void *arg) {
    (void)arg;  // Unused argument
    gpio_toggle(LED_PIN);  // Toggle the LED
    xtimer_set(&timer, TIMER_INTERVAL);  // Reset the timer
}

int main(void) {
    // Initialize the LED pin as output
    gpio_init(LED_PIN, GPIO_OUT);

    // Initialize the timer
    timer.callback = timer_callback;
    timer.arg = NULL;

    // Set the timer to trigger the callback every second
    xtimer_set(&timer, TIMER_INTERVAL);

    return 0;
}