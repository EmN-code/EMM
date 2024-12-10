#include <stdio.h>
#include <stdint.h>
#include <periph/gpio.h>
#include <xtimer.h>

#define LED_PIN GPIO_PIN(PORT_C, 8)
#define TIMER_INTERVAL (1U * US_PER_SEC)

static void timer_callback(void *arg) {
    (void)arg;
    // Toggle LED state
    gpio_toggle(LED_PIN);
    printf("LED toggled\n");
}

int main(void) {
    // Initialize the GPIO as output for the LED
    gpio_init(LED_PIN, GPIO_OUT);

    // Initialize timer structure
    xtimer_t timer;
    timer.callback = timer_callback;
    timer.arg = NULL;

    // Set and start the timer
    xtimer_set(&timer, TIMER_INTERVAL);

    // Our program will continue running
    while (1) {
        // Infinite loop to keep the main function running
    }

    return 0;
}