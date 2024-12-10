#include "xtimer.h"
#include "periph/gpio.h"

#define TIMER_INTERVAL (1U * US_PER_SEC)  // Timer interval set to 1 second
#define LED_PIN        GPIO_PIN(PORT_A, 5)  // Define the pin connected to the LED

void timer_callback(void *arg) {
    (void)arg;
    gpio_toggle(LED_PIN);  // Toggle the LED state
}

int main(void) {
    // Initialize the GPIO pin for the LED
    gpio_init(LED_PIN, GPIO_OUT);

    // Initialize a timer
    ztimer_t timer;
    timer.callback = timer_callback;
    timer.arg = NULL;

    // Set the timer to trigger every second
    ztimer_set_msg(ZTIMER_MSEC, &timer, TIMER_INTERVAL, NULL, 0);

    while (1) {
        // Main loop does nothing, just wait for timer events
    }

    return 0;
}