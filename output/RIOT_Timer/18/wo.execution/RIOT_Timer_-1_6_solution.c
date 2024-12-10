#include "xtimer.h"
#include "timex.h"
#include "ztimer.h"
#include "periph/gpio.h"
#include "msg.h"
#include <stdio.h>

#define LED_PIN GPIO_PIN(0, 2) // Example GPIO pin for LED

void toggle_led(void) {
    gpio_toggle(LED_PIN); // Toggle LED state
}

int main(void) {
    // Initialize the GPIO for the LED
    gpio_init(LED_PIN, GPIO_OUT);
    
    // Set up a repeating timer to toggle the LED every second
    ztimer_t timer;
    timer.callback = toggle_led;
    timer.interval = ztimer_usec_from_sec(1);
    timer.arg = NULL;
    
    ztimer_periodic_start(&timer);
    
    // Main loop
    while (1) {
        // Idle the CPU waiting for the timer interrupt
        msg_t msg;
        msg_receive(&msg);
    }
    return 0;
}