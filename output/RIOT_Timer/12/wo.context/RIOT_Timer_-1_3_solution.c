#include <ztimer.h>
#include <stdio.h>
#include <periph/gpio.h>

#define LED_PIN GPIO_PIN(0, 2) // Use appropriate GPIO pin for your setup

static ztimer_t timer;

void timer_callback(void *arg) {
    (void)arg;
    gpio_toggle(LED_PIN);
    ztimer_set(ZTIMER_MSEC, &timer, 1000); // Schedule next callback in 1 second
}

int main(void) {
    // Initialize the GPIO pin
    gpio_init(LED_PIN, GPIO_OUT);

    // Initialize the timer
    timer.callback = timer_callback;
    timer.arg = NULL;
    ztimer_set(ZTIMER_MSEC, &timer, 1000); // Schedule first callback in 1 second

    // Main loop
    while(1) {
        ztimer_sleep(ZTIMER_MSEC, 1000); // Sleep for 1 second
    }

    return 0;
}