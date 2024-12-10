#include <ztimer.h>
#include <stdio.h>
#include <periph/gpio.h>

#define LED_PIN GPIO_PIN(0, 10)  // Example pin, adjust as necessary
#define TIMER_INTERVAL (1U * US_PER_SEC) // Define interval as 1 second

void timer_handler(void)
{
    gpio_toggle(LED_PIN); // Toggle LED state
}

int main(void)
{
    gpio_init(LED_PIN, GPIO_OUT);

    ztimer_t timer;
    ztimer_set(&timer, TIMER_INTERVAL, timer_handler, NULL);

    while (1) {
        // Keep the program running
    }
    return 0;
}