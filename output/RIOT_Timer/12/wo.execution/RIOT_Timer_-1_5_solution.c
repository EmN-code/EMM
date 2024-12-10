#include "ztimer.h"
#include "ztimer/timeout.h"
#include "periph/gpio.h"

#define TIMER_INTERVAL (ZTIMER_MSEC_PER_SEC) // Sets the interval to 1 second
#define LED_PIN GPIO_PIN(0, 2) // Example pin definition, adjust as necessary

// Timer object
static ztimer_t timer;

void timer_callback(void *arg) {
    (void)arg; // Argument not used
    gpio_toggle(LED_PIN); // Toggles the LED
}

int main(void) {
    // Initialize LED pin
    gpio_init(LED_PIN, GPIO_OUT);

    // Initialize and set up the timer
    ztimer_set(ZTIMER_MSEC, &timer, TIMER_INTERVAL);
    timer.callback = timer_callback;
    timer.arg = NULL; // No custom argument needed

    // Start the timer
    ztimer_start(ZTIMER_MSEC, &timer);

    // Keep the program running
    while(1) {
        ztimer_sleep(ZTIMER_MSEC, TIMER_INTERVAL);
    }

    return 0;
}