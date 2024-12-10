#include <xtimer.h>
#include <stdio.h>
#include <periph/gpio.h>

// LED_PIN needs to correspond to the actual pin number used for the LED.
#define LED_PIN GPIO_PIN(PORT_A, 0)  // Use the correct port for PORT_A

void timer_callback(void *arg) {
    // Toggle the LED state
    (void) arg;
    gpio_toggle(LED_PIN);
    printf("LED toggled\n");
}

int main(void) {
    // Initialize the GPIO pin for the LED
    if (gpio_init(LED_PIN, GPIO_OUT) != 0) {
        printf("Error: Unable to initialize GPIO pin\n");
        return 1;
    }

    // Initialize the timer
    xtimer_ticks32_t interval = xtimer_ticks_from_usec(1000000);  // 1 second
    xtimer_t timer;
    timer.callback = timer_callback;
    timer.arg = NULL;

    while (1) {
        xtimer_set(&timer, interval);
        xtimer_usleep_until(&(timer.target), xtimer_usec_from_ticks(interval));
    }

    return 0;
}