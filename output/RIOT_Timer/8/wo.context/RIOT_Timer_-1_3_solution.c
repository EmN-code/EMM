#include <xtimer.h>
#include <timex.h>
#include <stdio.h>
#include <periph/gpio.h>

#define LED_PIN GPIO_PIN(0, 2)  // Use GPIO 2, typically LED_BUILTIN on ESP32
#define INTERVAL (1U * US_PER_SEC)  // 1 second interval

static void timer_callback(void *arg) {
    (void)arg;
    gpio_toggle(LED_PIN);  // Toggle the LED
}

int main(void) {
    // Initialize the GPIO pin
    gpio_init(LED_PIN, GPIO_OUT);

    // Initialize the timer
    xtimer_t timer;
    timer.callback = timer_callback;
    timer.arg = NULL;

    // Set the timer to trigger every second
    xtimer_set(&timer, INTERVAL);

    // Main loop does nothing, just let the timer interrupt handle the LED toggle
    while (1) {
        xtimer_sleep(1);
    }

    return 0;
}