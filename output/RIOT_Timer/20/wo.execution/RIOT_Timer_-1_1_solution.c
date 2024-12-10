#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <timex.h>
#include <ztimer.h>
#include <periph/gpio.h>

#define LED_PIN GPIO_PIN(PORT_B, 0)  // Define the LED pin according to your board specifications
#define TIMER_INTERVAL (1U * US_PER_SEC)  // Timer interval in microseconds

void timer_callback(void *arg) {
    (void)arg;  // Unused parameter
    static bool led_state = false;  // Initial LED state as false (off)

    // Toggle LED state
    led_state = !led_state;
    if (led_state) {
        gpio_set(LED_PIN);
    } else {
        gpio_clear(LED_PIN);
    }
}

int main(void) {
    // Initialize GPIO for LED
    gpio_init(LED_PIN, GPIO_OUT);

    // Timer variable
    ztimer_t timer;

    // Initialize the timer
    ztimer_set(ZTIMER_SEC, &timer, TIMER_INTERVAL, timer_callback, NULL);

    while (1) {
        // Sleep until the next interrupt
        ztimer_sleep(ZTIMER_SEC, TIMER_INTERVAL);
    }

    return 0;
}