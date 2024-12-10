#include <stdio.h>
#include "thread.h"
#include "timex.h"
#include "ztimer.h"
#include "periph/gpio.h"

#define LED_PIN GPIO_PIN(0, 2)  // Assuming the LED is connected to pin GPIO0_2

static ztimer_t blinker_timer;

// Timer callback function
static void timer_callback(void *arg) {
    (void)arg; // Unused
    if (gpio_read(LED_PIN)) {
        gpio_clear(LED_PIN);
    } else {
        gpio_set(LED_PIN);
    }
}

int main(void) {
    // Initialize the GPIO pin for output
    gpio_init(LED_PIN, GPIO_OUT);
    
    // Set up the timer to trigger every second
    ztimer_set(ZTIMER_MSEC, &blinker_timer, 1000, timer_callback, NULL);

    while (1) {
        // Optional: put the main thread to sleep to lower CPU usage
        thread_sleep();
    }

    return 0;
}