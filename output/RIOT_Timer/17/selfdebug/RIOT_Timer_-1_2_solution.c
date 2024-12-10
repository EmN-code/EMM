#include "ztimer.h"
#include "periph/gpio.h"
#include "board.h"

#define TIMER_INTERVAL (1U * ZTIMER_MSEC)

static void timer_callback(void *arg)
{
    (void)arg;
    gpio_toggle(GPIO_PIN(PORT_A, 0));  // Toggle the LED connected to PORT_A pin 0
}

int main(void)
{
    ztimer_t timer;

    // Initialize the GPIO pin for the LED
    gpio_init(GPIO_PIN(PORT_A, 0), GPIO_OUT);

    // Set a periodic timer
    ztimer_set(&timer, TIMER_INTERVAL);
    timer.callback = timer_callback;
    timer.arg = NULL;

    // Start the timer
    ztimer_start(&timer);

    while (1) {
        // Put the main thread to sleep to save power
        ztimer_sleep(ZTIMER_MSEC, TIMER_INTERVAL);
    }
    return 0;
}