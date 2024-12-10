#include <xtimer.h>
#include <stdio.h>
#include <periph/gpio.h>

#define LED_PIN GPIO_PIN(0, 2) // Define the GPIO pin connected to the LED

void timer_callback(void *arg)
{
    (void)arg;
    gpio_toggle(LED_PIN); // Toggle the LED state
    printf("LED toggled\n");
}

int main(void)
{
    // Initialize the LED pin as an output
    gpio_init(LED_PIN, GPIO_OUT);

    // Define an xtimer instance
    xtimer_t timer;
    timer.callback = timer_callback;
    timer.arg = NULL;

    // Set the timer to trigger every second (1,000,000 microseconds)
    xtimer_set(&timer, 1000000);

    // Keep the program running indefinitely
    while (1) {
        xtimer_sleep(1);
    }

    return 0;
}