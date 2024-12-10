#include <xtimer.h>
#include <periph/gpio.h> 
#include <stdio.h>

#define TIMER_INTERVAL (1U * US_PER_SEC) // Timer interval of 1 second
#define LED_PIN        GPIO_PIN(0, 2)    // Assuming GPIO pin 2 is connected to an LED

static xtimer_ticks32_t xt;

void callback(void *arg) {
    (void)arg;
    // Toggle LED pin
    gpio_toggle(LED_PIN);
}

int main(void) {
    // Initialize the LED pin as output
    gpio_init(LED_PIN, GPIO_OUT);
    
    // Set the timer's callback argument to null (as it's not used here)
    xtimer_ticks32_t interval = xtimer_ticks32(TIMER_INTERVAL);
    
    while (1) {
        // Set a timer to call the callback every second
        xtimer_set_cb(&xt, interval, callback, NULL);
        xtimer_spin_until(xtimer_now() + interval);
    }

    return 0;
}