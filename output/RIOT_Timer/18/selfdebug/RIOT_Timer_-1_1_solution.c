#include <timex.h>
#include <stdio.h>
#include <periph/gpio.h>
#include <periph/timer.h>

#define LED_PIN GPIO_PIN(0, 2) // Assuming LED is connected on GPIO0, pin 2

void timer_callback(void *arg) {
    (void)arg;
    static int led_state = 0;

    if (led_state) {
        gpio_clear(LED_PIN);
    } else {
        gpio_set(LED_PIN);
    }
    led_state = !led_state;
}

int main(void) {
    printf("Starting LED blink timer\n");

    // Initialize LED pin
    gpio_init(LED_PIN, GPIO_OUT);

    // Timer configuration
    tim_t dev = TIMER_DEV(0);
    timer_init(dev, 1, timer_callback, NULL);
    timer_set(dev, 1000000, true);
    timer_start(dev);

    while (1) {
        // Main loop does nothing, just waits for interruption
    }

    return 0;
}