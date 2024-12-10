#include "xtimer.h"
#include "timex.h"
#include "periph/gpio.h"

#define TIMER_INTERVAL (1U * US_PER_SEC) // Timer interval of 1 second

// Define the GPIO pin for the LED
#define LED_PIN GPIO_PIN(0, 2)  // Assuming an LED is connected to GPIO0 pin 2

// Callback function to toggle the LED
void timer_callback(void *arg) {
    (void)arg;
    gpio_toggle(LED_PIN);
}

int main(void) {
    // Initialize the GPIO pin
    gpio_init(LED_PIN, GPIO_OUT);

    // Define an xtimer object with a callback
    xtimer_t timer;
    timer.callback = timer_callback;
    timer.arg = NULL;

    // Initialize and set the timer
    while (1) {
        xtimer_set(&timer, TIMER_INTERVAL);
        xtimer_sleep(TIMER_INTERVAL);
    }

    return 0;
}