
#include <stdlib.h>
#include <stdio.h>
#include <xtimer.h>
#include <periph/gpio.h>

#define LED_PIN GPIO_PIN(0, 16)  // Example LED pin for ESP32
#define TIMER_INTERVAL (1 * US_PER_SEC)  // 1 second interval

static xtimer_t timer;
static bool led_state = false;

void timer_callback(void) {
    gpio_write(LED_PIN, led_state ? GPIO_HIGH : GPIO_LOW);
    led_state = !led_state;
    xtimer_set(&timer, TIMER_INTERVAL);
}

int main(void) {
    // Initialize the LED pin as output
    gpio_init(LED_PIN, GPIO_OUT);

    // Set the timer to trigger the callback function
    xtimer_set(&timer, TIMER_INTERVAL);

    // Main loop
    while (1) {
        xtimer_usleep(100000);  // Sleep for 100 ms to avoid busy-waiting
    }

    return 0;
}
