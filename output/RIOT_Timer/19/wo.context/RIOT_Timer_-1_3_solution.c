#include <xtimer.h>
#include <timex.h>
#include <periph/gpio.h>

#define TIMER_INTERVAL (1U * US_PER_SEC)
#define LED_PIN        GPIO_PIN(0, 2) // Assuming GPIO Port 0, Pin 2 for the LED

static void timer_callback(void *arg)
{
    (void)arg;
    gpio_toggle(LED_PIN);
}

int main(void)
{
    // Initialize the LED pin
    gpio_init(LED_PIN, GPIO_OUT);
    
    static xtimer_t timer;
    timer.callback = timer_callback;
    timer.arg = NULL;
    
    // Set the timer to trigger every second
    xtimer_set(&timer, TIMER_INTERVAL);
    
    return 0;
}