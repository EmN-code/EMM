
#include "xtimer.h"
#include "periph/gpio.h"

#define LED_PIN GPIO_PIN(0, 16)  // Assuming GPIO 16 is connected to an LED on ESP32

void toggle_led(void) {
    gpio_toggle(LED_PIN);
}

int main(void) {
    // Initialize the LED pin as output
    gpio_init(LED_PIN, GPIO_OUT);

    // Create a timer that triggers every second
    xtimer_t timer;
    uint32_t period = 1000000;  // 1 second in microseconds

    // Start the timer
    xtimer_init(&timer);
    xtimer_set(&timer, period, (void (*)(void *))toggle_led, NULL);

    // The main loop can be empty as the timer will handle the LED toggling
    while (1) {
        xtimer_usleep(1000000);  // Sleep for 1 second to avoid busy-waiting
    }

    return 0;
}
