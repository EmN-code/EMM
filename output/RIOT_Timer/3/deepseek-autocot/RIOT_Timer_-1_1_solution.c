
#include "xtimer.h"
#include "periph/gpio.h"

#define LED_PIN GPIO_PIN(0, 13)  // Assuming GPIO 13 is connected to an LED on ESP32

void timer_callback(void *arg) {
    (void)arg;  // Unused argument
    gpio_toggle(LED_PIN);  // Toggle the LED state
}

int main(void) {
    // Initialize the LED pin as output
    gpio_init(LED_PIN, GPIO_OUT);

    // Create a timer that triggers every second
    xtimer_t timer;
    uint32_t period = 1000000;  // 1 second in microseconds

    // Initialize the timer and set the callback function
    xtimer_init(&timer);
    xtimer_set(&timer, period, timer_callback, NULL);

    // Main loop (nothing to do here, the timer will handle the LED toggling)
    while (1) {
        xtimer_usleep(1000000);  // Sleep for 1 second to avoid busy-waiting
    }

    return 0;
}