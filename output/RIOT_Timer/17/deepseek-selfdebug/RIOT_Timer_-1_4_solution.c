
#include "xtimer.h"
#include "periph/gpio.h"

#define LED_PIN GPIO_PIN(0, 13)  // Assuming GPIO 13 on ESP32

xtimer_t timer;

void timer_callback(void *arg) {
    (void)arg;
    gpio_toggle(LED_PIN);  // Toggle the LED
    xtimer_set(&timer, 1 * US_PER_SEC);  // Set the timer to trigger again in 1 second
}

int main(void) {
    gpio_init(LED_PIN, GPIO_OUT);  // Initialize the LED pin as output
    xtimer_init();  // Initialize the xtimer

    timer.callback = timer_callback;  // Set the callback function
    timer.arg = NULL;  // No argument for the callback

    xtimer_set(&timer, 1 * US_PER_SEC);  // Set the timer to trigger in 1 second

    return 0;
}
