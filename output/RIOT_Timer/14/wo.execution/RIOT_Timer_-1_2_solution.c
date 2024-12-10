#include <xtimer.h>  // Timer related functions
#include <stdio.h>   // Standard input/output
#include <periph/gpio.h>  // GPIO functions

#define LED_PIN GPIO_PIN(0, 2)  // Assuming the LED is connected to GPIO PIN 2

void timer_callback(void *arg) {
    (void)arg;
    // Toggle the LED state
    gpio_toggle(LED_PIN);
    printf("LED toggled!\n");
}

int main(void) {
    // Initialize the LED pin
    gpio_init(LED_PIN, GPIO_OUT);

    // Initialize the timer
    xtimer_ticks32_t interval = xtimer_seconds(1);
    xtimer_t timer;
    timer.callback = timer_callback;
    timer.arg = NULL;

    // Start the timer. It will call the `timer_callback` function every second.
    while (1) {
        xtimer_set(&timer, interval);
        xtimer_sleep(1);  // Sleep for a second to allow the timer to trigger
    }

    return 0;
}