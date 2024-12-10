
#include "xtimer.h"
#include "periph/gpio.h"

#define LED_PIN GPIO_PIN(0, 2)  // Assuming GPIO0, pin 2 is the LED pin on ESP32

void timer_callback(void *arg) {
    (void)arg;  // Unused argument
    gpio_toggle(LED_PIN);  // Toggle the LED
}

int main(void) {
    // Initialize the LED pin as output
    gpio_init(LED_PIN, GPIO_OUT);

    // Initialize the xtimer
    xtimer_init();

    // Create a timer
    xtimer_t timer;
    timer.callback = timer_callback;
    timer.arg = NULL;

    // Set the timer to trigger every second
    xtimer_set(&timer, 1 * 1000 * 1000);  // 1 second in microseconds

    return 0;
}
