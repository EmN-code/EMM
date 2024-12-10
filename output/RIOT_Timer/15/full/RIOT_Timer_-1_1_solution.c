#include "xtimer.h"
#include "periph/gpio.h"

#define INTERVAL (1U * US_PER_SEC) // 1 second interval
#define LED_PIN  GPIO_PIN(PORT_A, 0) // Assuming LED is connected to PORT_A pin 0

void timer_callback(void *arg)
{
    (void)arg;
    // Toggle the LED state
    gpio_toggle(LED_PIN);
}

int main(void)
{
    // Initialize the LED pin
    if (gpio_init(LED_PIN, GPIO_OUT)) {
        printf("Error: Unable to initialize LED pin\n");
        return 1;
    }

    // Initialize the timer
    xtimer_ticks32_t interval = { .ticks32 = INTERVAL };
    while (1) {
        // Sleep for 1 second
        xtimer_periodic_wakeup(&interval, INTERVAL);
        // Trigger the callback function
        timer_callback(NULL);
    }

    return 0;
}