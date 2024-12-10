#include <xtimer.h>
#include <stdio.h>
#include <timex.h>
#include <periph/gpio.h>

#define TIMER_INTERVAL (1 * US_PER_SEC) // 1 second in microseconds
#define LED_PIN        GPIO_PIN(PORT_A, 0) // Define LED pin on port A, pin 0

static gpio_t led = LED_PIN;

void timer_callback(void *arg)
{
    (void)arg;
    gpio_toggle(led); // Toggle LED state
    printf("LED toggled\n");
}

int main(void)
{
    // Initialize the LED pin
    if (gpio_init(led, GPIO_OUT)) {
        printf("Failed to initialize GPIO\n");
        return 1;
    }
    
    // Setup timer
    xtimer_t timer;
    timer.callback = timer_callback;
    timer.arg = NULL;
    
    // Start timer
    xtimer_set(&timer, TIMER_INTERVAL);
    
    // Main loop
    while (1) {
        xtimer_sleep(1); // Sleep for a while to reduce CPU usage
    }
    
    return 0;
}