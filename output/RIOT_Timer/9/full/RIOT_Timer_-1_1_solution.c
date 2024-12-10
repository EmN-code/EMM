#include <timers/timer.h>
#include <stdio.h>
#include <periph/gpio.h>

#define LED_PIN GPIO_PIN(PORT_A, 0)  // Assuming LED is connected to Port A, Pin 0

event_t led_event;

// Timer callback function
static void timer_callback(void *arg)
{
    (void)arg;
    // Toggle the LED state
    gpio_toggle(LED_PIN);
    printf("LED toggled\n");
}

// Main function
int main(void)
{
    // Initialize GPIO for LED
    if (gpio_init(LED_PIN, GPIO_OUT)) {
        printf("Error initializing GPIO\n");
        return 1;
    }

    // Initialize the timer
    if (timer_init(TIMER_DEV(0), 1000000, timer_callback, NULL) != 0) {
        printf("Timer initialization failed\n");
        return 1;
    }

    // Start the timer
    timer_start(TIMER_DEV(0));

    while (1) {
        // Loop forever, letting the timer callback handle LED toggling
        xtimer_sleep(1);
    }

    return 0;
}