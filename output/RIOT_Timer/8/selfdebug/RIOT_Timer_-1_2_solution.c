#include <timex.h>
#include <stdio.h>
#include <periph/gpio.h>

#define LED_PIN GPIO_PIN(0, 2) // Representing GPIO0, pin 2

static const timex_t INTERVAL = 1U * US_PER_SEC; // 1-second interval

static xtimer_t timer;

static void timer_callback(void *arg) {
    (void)arg;
    static bool led_state = false;

    if (led_state) {
        gpio_clear(LED_PIN);
    } else {
        gpio_set(LED_PIN);
    }

    led_state = !led_state;

    // Reset the timer for the next cycle
    xtimer_set(&timer, INTERVAL);
}

int main(void) {
    // Initialize the LED pin as an output
    gpio_init(LED_PIN, GPIO_OUT);

    // Initialize and start the timer
    timer.callback = timer_callback;
    timer.arg = NULL;

    xtimer_set(&timer, INTERVAL);

    while (1) {
        // Main loop doing nothing, waiting for timer interrupts
        __asm__ __volatile__ ("nop");
    }

    return 0;
}