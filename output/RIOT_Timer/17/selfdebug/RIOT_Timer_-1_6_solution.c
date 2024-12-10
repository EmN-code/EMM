#include "xtimer.h"
#include "periph/gpio.h"

#define TIMER_INTERVAL (1U * US_PER_SEC) // 1 second in microseconds
#define LED_PIN GPIO_PIN(PORT_A, 1) // Assuming GPIO port A, pin 1 is used for the LED

void timer_callback(void *arg) {
    (void)arg;
    static int led_state = 0;
    led_state = !led_state; // Toggle state
    gpio_write(LED_PIN, led_state);
}

int main(void) {
    // Initialize the GPIO pin for output
    gpio_init(LED_PIN, GPIO_OUT);
    
    // Set up a recurring timer
    xtimer_t timer;
    timer.callback = timer_callback;
    timer.arg = NULL;

    // Start the timer
    xtimer_set(&timer, TIMER_INTERVAL);

    // Main loop
    while (1) {
        // Put the MCU to sleep until the next interrupt
        xtimer_sleep(TIMER_INTERVAL);
    }

    return 0;
}