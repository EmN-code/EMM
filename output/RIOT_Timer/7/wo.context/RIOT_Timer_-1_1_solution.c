#include "timer.h"
#include "periph/gpio.h"
#include "periph/timer.h"
#include "xtimer.h"
#include <stdio.h>

#define TIMER_INTERVAL (1000000) // Timer interval of 1 second in microseconds.
#define LED_PIN GPIO_PIN(PORT_A, 0) // Example LED pin, adjust for your board.

static void timer_cb(void *arg)
{
    (void)arg;
    gpio_toggle(LED_PIN); // Toggle the LED state.
    printf("LED toggled!\n");
}

int main(void)
{
    gpio_init(LED_PIN, GPIO_OUT);
    tim_t dev = TIMER_DEV(0);
    timer_set(dev, TIMER_INTERVAL, timer_cb, NULL);
    timer_start(dev); // Start the timer.

    while (1) {
        // Keep running to allow timer interruption.
    }
    return 0;
}