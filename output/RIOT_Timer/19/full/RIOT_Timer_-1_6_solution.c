#include <xtimer.h>
#include <stdio.h>
#include <periph/gpio.h>

#define LED_PIN GPIO_PIN(PORT_A, 5) // Define the correct pin for your LED
#define TIMER_INTERVAL (1U * US_PER_SEC) // Timer interval in microseconds for 1 second

static void timer_callback(void *arg) {
    (void)arg;
    // Toggle the LED pin state
    gpio_toggle(LED_PIN);
}

int main(void) {
    // Initialize the LED pin
    gpio_init(LED_PIN, GPIO_OUT);

    // Create a timer variables
    xtimer_t timer;
    timer.callback = timer_callback;
    timer.arg = NULL;

    while(1) {
        // Set the timer to trigger every second
        xtimer_set(&timer, TIMER_INTERVAL);
        printf("LED Toggled\n");
        xtimer_sleep(1);
    }

    return 0;
}