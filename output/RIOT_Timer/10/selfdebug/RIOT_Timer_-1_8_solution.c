#include <xtimer.h>
#include <periph/gpio.h>
#include <stdio.h>

#define LED_PIN  GPIO_PIN(0, 2)
#define TIMER_INTERVAL (1U * 1000000U) // 1 second in microseconds

static void timer_callback(void *arg)
{
    (void)arg;
    gpio_toggle(LED_PIN);
    printf("LED toggled\n");
}

int main(void)
{
    // Initialize the GPIO pin
    if (gpio_init(LED_PIN, GPIO_OUT)) {
        printf("Error initializing GPIO pin\n");
        return 1;
    }
    printf("Successfully initialized GPIO pin\n");

    // timer variable
    xtimer_t timer;

    // Set the timer to trigger every second
    xtimer_init(&timer);
    xtimer_set(&timer, TIMER_INTERVAL, timer_callback, NULL);

    printf("Timer started\n");

    // Keep the main function running
    while (1) {
        // Do nothing, just waiting for the interrupt
    }

    return 0;
}