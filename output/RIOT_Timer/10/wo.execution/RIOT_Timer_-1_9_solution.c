#include <xtimer.h>
#include <stdio.h>
#include <periph/gpio.h> 

#define TIMER_INTERVAL (1U * US_PER_SEC)
#define LED_PIN        GPIO_PIN(PORT_A, 5)

static xtimer_t timer;

void timer_callback(void *arg)
{
    (void)arg;  // Avoid unused parameter warning
    gpio_toggle(LED_PIN);
}

int main(void)
{
    // Initialize the LED pin
    gpio_init(LED_PIN, GPIO_OUT);
    
    // Initialize the timer
    timer.callback = timer_callback;
    timer.arg = NULL;

    // Set the timer with specified interval
    xtimer_set(&timer, TIMER_INTERVAL);

    // Main thread can perform other operations, just sleep
    while (1) {
        xtimer_sleep(1);
    }
    return 0;
}