#include <stdio.h>  // Standard input/output library for debugging
#include <periph/gpio.h>  // GPIO peripheral interface
#include <xtimer.h>  // xtimer for timer struct and functions

#define LED_PIN GPIO_PIN(0, 0)  // Definition for the LED pin

void toggle_led(void) {
    gpio_toggle(LED_PIN);
}

int main(void) {
    // Initialize GPIO
    gpio_init(LED_PIN, GPIO_OUT);
    
    // Define the timer
    xtimer_ticks32_t timer_interval = xtimer_ticks_from_ms(1000); // 1-second interval
    
    while(1) {
        // Trigger the function every second
        toggle_led();
        xtimer_periodic_wakeup(NULL, timer_interval);
    }

    return 0;
}