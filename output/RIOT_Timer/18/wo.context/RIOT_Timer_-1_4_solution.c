#include "xtimer.h"
#include "periph/gpio.h"
#include <stdio.h>

#define LED_PIN GPIO_PIN(PORT_A, 0)  // Define the pin for LED (Example: PORT_A pin 0)
#define TIMER_INTERVAL (1U * US_PER_SEC)  // Set timer interval for 1 second

void timer_callback(void *arg)
{
    (void)arg;
    int current_state = gpio_read(LED_PIN);
    gpio_write(LED_PIN, !current_state);  // Toggle LED state
    printf("LED is %s\n", current_state ? "Off" : "On");
}

int main(void)
{
    gpio_init(LED_PIN, GPIO_OUT);  // Initialize LED pin as output

    static xtimer_t timer;
    timer.callback = timer_callback;
    timer.arg = NULL;

    xtimer_set(&timer, TIMER_INTERVAL);  // Set the timer with the defined interval

    while (1) {
        // Main loop can be used for other tasks
        xtimer_sleep(1);
    }

    return 0;
}