#include <xtimer.h>
#include <stdio.h>
#include <periph/gpio.h>

// Define the LED pin
#define LED_PIN GPIO_PIN(PORT_A, 2)

// Timer interval (1 second)
#define TIMER_INTERVAL (1U * US_PER_SEC)

// Timer ISR callback function to toggle LED
void timer_callback(void *arg) {
    (void)arg;
    gpio_toggle(LED_PIN);
    printf("LED toggled\n");
}

int main(void) {
    // Initialize the LED pin
    if (gpio_init(LED_PIN, GPIO_OUT)) {
        printf("Error initializing GPIO\n");
        return 1;
    }

    printf("Starting LED blink example with timer\n");

    // Configure timer
    xtimer_t timer;
    xtimer_set(&timer, TIMER_INTERVAL);
    timer.callback = timer_callback;
    timer.arg = NULL;

    // Start the timer with error checking
    while (true) {
        xtimer_set(&timer, TIMER_INTERVAL);
        if (xtimer_set_msg(&timer, TIMER_INTERVAL, &timer.callback) < 0) {
            printf("Error setting timer\n");
            xtimer_sleep(1); // Wait before retry
            continue;
        }
        break;
    }

    // Enter main loop (infinite loop)
    while (1) {
        xtimer_sleep(TIMER_INTERVAL);
    }

    return 0;
}