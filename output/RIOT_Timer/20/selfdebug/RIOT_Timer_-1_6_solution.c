#include <xtimer.h>
#include <ztimer.h>
#include <periph/gpio.h>

#define LED_PIN GPIO_PIN(0, 2) // Example GPIO pin, adjust if necessary

void timer_callback(void *arg) {
    (void)arg;
    gpio_set(LED_PIN); // Turn on the LED
    // Add code to turn off the LED if needed, e.g., using gpio_clear after some delay
}

int main(void) {
    gpio_init(LED_PIN, GPIO_OUT);

    ztimer_t timer;
    timer.callback = timer_callback;
    timer.arg = NULL;

    // Set up the timer to expire every second
    ztimer_set(ZTIMER_USEC, &timer, ztimer_usec_from_ticks(1000000));

    while (1) {
        // Main loop of your program, can go to sleep or perform other tasks
    }
    return 0;
}