
#include "xtimer.h"
#include "periph/gpio.h"

#define LED_PIN GPIO_PIN(0, 16)  // Assuming GPIO0, pin 16 is connected to an LED
#define TIMER_INTERVAL (1 * 1000 * 1000U)  // 1 second in microseconds

static xtimer_t timer;

void timer_callback(void *arg) {
    (void)arg;  // Unused argument
    gpio_toggle(LED_PIN);
    xtimer_set(&timer, TIMER_INTERVAL);
}

int main(void) {
    // Initialize the LED pin as output
    gpio_init(LED_PIN, GPIO_OUT);

    // Initialize the timer
    xtimer_init();

    // Set the timer to trigger the callback every second
    xtimer_set(&timer, TIMER_INTERVAL);

    // Start the timer with the callback function
    xtimer_set_cb(&timer, timer_callback, NULL);

    return 0;
}
