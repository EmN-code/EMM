#include <stdio.h>
#include <ztimer.h>
#include <timex.h>
#include <periph/gpio.h>

// Function to be called when the timer triggers
static void timer_callback(void *arg) {
    (void)arg;  // Argument is unused
    // Toggle the LED
    gpio_toggle(GPIO_PIN(PORT_A, 0));
}

int main(void) {
    // Initialize the GPIO pin connected to the LED
    gpio_init(GPIO_PIN(PORT_A, 0), GPIO_OUT);

    // Initialize the ztimer for millisecond precision
    ztimer_ticks_t interval = MS_PER_SEC; // 1000 ms
    ztimer_t timer;
    // Set the callback function
    timer.callback = timer_callback;
    timer.arg = NULL;
    // Start the timer
    ztimer_set(ZTIMER_MSEC, &timer, interval);

    // Main loop does nothing, timer is handled in the callback
    while (1) {
        xtimer_sleep(1);
    }
    return 0;
}