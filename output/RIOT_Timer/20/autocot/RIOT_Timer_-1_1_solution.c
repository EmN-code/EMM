#include <stdio.h>
#include "xtimer.h"
#include "periph/gpio.h"

#define LED_PIN GPIO_PIN(0, 2) // Example: GPIO pin where the LED is connected
#define TIMER_INTERVAL 1000000 // Timer interval in microseconds (1 second)

static void timer_callback(void *arg)
{
    (void)arg;
    static bool led_state = false;
    
    if (led_state) {
        gpio_clear(LED_PIN);
    } else {
        gpio_set(LED_PIN);
    }
    led_state = !led_state;

    printf("LED is %s\n", led_state ? "ON" : "OFF");
}

int main(void)
{
    // Initialize the GPIO pin
    if (gpio_init(LED_PIN, GPIO_OUT)) {
        printf("Error: Failed to initialize GPIO pin\n");
        return 1;
    }

    // Initialize the timer
    xtimer_t timer;
    xtimer_set(&timer, TIMER_INTERVAL, timer_callback, NULL);

    while (1) {
        // Let the timer callback do the work
        xtimer_usleep(TIMER_INTERVAL);
    }

    return 0;
}