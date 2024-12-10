#include "xtimer.h"  // For xtimer functionalities
#include "stdio.h"   // For printf
#include "periph/gpio.h"  // For GPIO peripheral access

#define TIMER_INTERVAL (1U * US_PER_SEC) // 1 second interval
#define LED_PIN        GPIO_PIN(PORT_A, 5)  // Pin for LED (adjust according to your board)

static void timer_callback(void *arg) {
    (void)arg;
    static int led_state = 0;
    led_state = !led_state;

    if (led_state) {
        gpio_set(LED_PIN);    // Turn LED on
        printf("LED turned ON\n");
    }
    else {
        gpio_clear(LED_PIN);  // Turn LED off
        printf("LED turned OFF\n");
    }
}

int main(void) {
    // Initialize the LED pin
    gpio_init(LED_PIN, GPIO_OUT);

    // Setup timer
    xtimer_t timer;
    timer.callback = timer_callback;
    timer.arg = NULL;

    // Initialize and set the timer
    xtimer_set(&timer, TIMER_INTERVAL);

    while (1) {
        // Main loop performs other tasks or just yield
        xtimer_sleep(TIMER_INTERVAL);
    }

    return 0;
}