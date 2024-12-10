#include <xtimer.h>
#include <stdio.h>
#include <periph/gpio.h>

#define LED_PIN GPIO_PIN(PORT_X, PIN_Y)

void initialize_led(void) {
    // Initialize the GPIO pin for LED
    gpio_init(LED_PIN, GPIO_OUT);
}

void toggle_led(void) {
    // Toggle the LED state
    gpio_toggle(LED_PIN);
}

static void timer_callback(void *arg) {
    (void)arg;
    toggle_led();
}

int main(void) {
    initialize_led();

    // Define the timer
    xtimer_t timer;

    // Set the callback and interval in microseconds (1 second)
    timer.callback = timer_callback;
    timer.arg = NULL;
    timer.absolute = false;
    xtimer_set(&timer, xtimer_usec_from_ticks(1000000));

    while (1) {
        // Let the CPU go to sleep until the next interrupt occurs
        xtimer_sleep(60);
    }

    return 0;
}