#include <xtimer.h>   // For using timers
#include <stdio.h>    // For standard input and output in C
#include <periph/gpio.h>  // For GPIO operations

#define TIMER_INTERVAL (1U * US_PER_SEC) // 1 second interval
#define LED_PIN GPIO_PIN(0, 2) // Assuming LED is connected to port 0, pin 2

void _timer_callback(void *arg) {
    (void)arg;
    gpio_toggle(LED_PIN); // Toggle the LED state
}

int main(void) {
    // Initialize GPIO pin
    gpio_init(LED_PIN, GPIO_OUT);

    // Define timer variable
    xtimer_t timer;
    timer.callback = _timer_callback;
    timer.arg = NULL;

    // Set timer with configured interval
    xtimer_set(&timer, TIMER_INTERVAL);

    // Wait indefinitely
    while (1) {
        xtimer_sleep(1);
    }

    return 0;
}
