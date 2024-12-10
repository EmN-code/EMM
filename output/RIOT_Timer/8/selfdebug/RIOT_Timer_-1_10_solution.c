#include "xtimer.h"
#include "timex.h"
#include "thread.h"
#include "periph/gpio.h"
#include <stdio.h>

#define LED_PIN GPIO_PIN(PORT_B, 0)
#define INTERVAL 1

static void timer_callback(void *arg) {
    (void)arg;
    // Turn on LED
    gpio_set(LED_PIN);
    // Simulate delay for 1 second (1,000,000 microseconds) using xtimer
    xtimer_usleep(INTERVAL * US_PER_SEC);
    // Turn off LED
    gpio_clear(LED_PIN);
}

int main(void) {
    // Initialize the LED pin as output
    gpio_init(LED_PIN, GPIO_OUT);
    
    // Set up the timer structure
    xtimer_t timer;
    timer.callback = timer_callback;
    timer.arg = NULL;

    // This sets the timer to trigger every second
    xtimer_set(&timer, INTERVAL * US_PER_SEC);

    // Let the main thread sleep (simulate idle state)
    while (1) {
        xtimer_sleep(INTERVAL);
    }

    return 0;
}