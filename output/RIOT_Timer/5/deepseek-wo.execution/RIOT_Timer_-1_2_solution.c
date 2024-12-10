
#include "xtimer.h"
#include "periph/gpio.h"
#include "stdio.h"
#include "timex.h"

#define LED_PIN GPIO_PIN(0, 13)  // Assuming GPIO 13 is connected to an LED on ESP32
#define TIMER_INTERVAL (1 * US_PER_SEC)  // 1 second interval

static xtimer_t timer;

void timer_callback(void *arg) {
    (void)arg;  // Unused parameter
    gpio_toggle(LED_PIN);  // Toggle the LED
    xtimer_set(&timer, TIMER_INTERVAL);  // Reset the timer
}

int main(void) {
    // Initialize the LED pin as output
    gpio_init(LED_PIN, GPIO_OUT);

    // Set up the timer to trigger the callback every second
    xtimer_set(&timer, TIMER_INTERVAL);

    // Start the timer with the callback function
    timer.callback = timer_callback;

    // Infinite loop to keep the program running
    while (1) {
        xtimer_sleep(1);  // Sleep to avoid busy-waiting
    }

    return 0;
}
