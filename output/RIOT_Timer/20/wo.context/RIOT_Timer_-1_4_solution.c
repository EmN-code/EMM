#include <xtimer.h>
#include <stdio.h>
#include <periph/gpio.h>

#define LED_PIN GPIO_PIN(0, 2) // Adjust pin as per your board configuration
#define TIMER_INTERVAL (1U * US_PER_SEC) // 1 second interval

void timer_callback(void *arg) {
    (void)arg;
    // Toggle the LED state
    gpio_toggle(LED_PIN);
}

int main(void) {
    // Initialize LED pin
    gpio_init(LED_PIN, GPIO_OUT);

    // Set up a recurring timer with a 1-second interval
    xtimer_ticks32_t last_wakeup = xtimer_now();
    while (1) {
        // Wait for the next time interval
        xtimer_periodic_wakeup(&last_wakeup, TIMER_INTERVAL);
        // Execute the callback function
        timer_callback(NULL);
    }

    return 0;
}