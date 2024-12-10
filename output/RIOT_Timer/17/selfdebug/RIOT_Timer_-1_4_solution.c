#include <timex.h>
#include <stdio.h>
#include <periph/gpio.h>

#define TIMER_INTERVAL (1U * US_PER_SEC) // 1 second

static void timer_callback(void *arg) {
    // Toggle LED state
    gpio_toggle(GPIO_PIN(1, 0)); // Assuming LED is connected to port 1, pin 0
    (void)arg;
}

int main(void) {
    // Initialize the GPIO pin for the LED
    if (gpio_init(GPIO_PIN(1, 0), GPIO_OUT) < 0) {
        printf("Error initializing GPIO pin\n");
        return 1;
    }

    // Set up a timer
    xtimer_ticks32_t interval = xtimer_usec_to_ticks(TIMER_INTERVAL);
    while (1) {
        // Wait for the interval period
        xtimer_periodic_wakeup(xtimer_now(), interval);
        // Call the timer callback
        timer_callback(NULL);
    }

    return 0;
}