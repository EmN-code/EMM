#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <xtimer.h>
#include <timex.h>
#include <periph/gpio.h>

#define LED_PIN GPIO_PIN(0, 2) // Use appropriate GPIO port and pin for ESP32 LED

timer_t led_timer;

// Callback function for the timer
void timer_callback(void *arg) {
    (void)arg; // avoid unused parameter warning
    gpio_toggle(LED_PIN); // Toggle the LED state
}

int main(void) {
    printf("RIOT application starting\n");

    // Initialize the LED pin
    gpio_init(LED_PIN, GPIO_OUT); // Set as output

    // Initialize the timer and configure it to fire every second
    ztimer_t timer;
    ztimer_set(&timer, ZTIMER_MSEC_BASE, timer_callback, NULL, MS_PER_SEC);

    while (1) {
        // Keeping the main thread alive
        xtimer_sleep(1);
    }

    return 0;
}