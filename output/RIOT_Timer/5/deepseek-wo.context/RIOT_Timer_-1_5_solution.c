
#include "xtimer.h"
#include "periph/gpio.h"

#define LED_PIN GPIO_PIN(0, 16)  // Assuming GPIO 16 on port 0 is connected to an LED

void timer_callback(void *arg) {
    (void)arg;  // Unused argument
    gpio_toggle(LED_PIN);  // Toggle the LED state
}

int main(void) {
    // Initialize the LED pin as output
    gpio_init(LED_PIN, GPIO_OUT);

    // Create a timer
    xtimer_t timer;
    xtimer_init(&timer);

    // Set the timer to trigger every second
    while (1) {
        xtimer_sleep(1);  // Sleep for 1 second
        timer_callback(NULL);  // Call the callback function
    }

    return 0;
}
