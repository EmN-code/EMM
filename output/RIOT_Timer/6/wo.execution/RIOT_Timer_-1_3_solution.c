#include "ztimer.h"
#include "thread.h"
#include "periph/gpio.h"

#define LED_PIN GPIO_PIN(PORT_A, 5)  // Ensure correct definition as per your board setup
#define DELAY 1U  // in seconds

static ztimer_t timer;

void timer_callback(void *arg) {
    (void)arg;  // Unused parameter
    // Toggle the state of the LED pin
    gpio_toggle(LED_PIN);
}

int main(void) {
    // Initialize the GPIO pin
    if (gpio_init(LED_PIN, GPIO_OUT)) {
        printf("Error initializing GPIO\n");
        return 1;
    }

    // Set up the timer
    ztimer_set(ZTIMER_MSEC, &timer, DELAY * MS_PER_SEC, timer_callback, NULL);

    // Keep running and handle the timer
    while (1) {
        ztimer_periodic_wakeup(ZTIMER_MSEC, 1000);
    }
    return 0;
}