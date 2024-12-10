#include "timer.h"
#include "thread.h"
#include "periph/gpio.h"
#include "xtimer.h"

#define LED_PIN GPIO_PIN(0, 2)  // Example pin

tim_t timer;

void timer_callback(void *arg) {
    (void)arg;
    gpio_toggle(LED_PIN);
}

int main(void) {
    // Initialize the GPIO
    gpio_init(LED_PIN, GPIO_OUT);

    // Initialize the timer
    timer_t timer;
    if (timer_init(&timer, 1, timer_callback, NULL) != 0) {
        printf("Failed to initialize timer\n");
        return 1;
    }

    while (1) {
        // Wait for a second
        xtimer_sleep(1);

        // Trigger the timer
        timer_set(&timer, 1);
    }

    return 0;
}