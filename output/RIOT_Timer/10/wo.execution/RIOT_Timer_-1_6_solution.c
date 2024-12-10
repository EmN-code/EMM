#include <xtimer.h>
#include <periph/gpio.h>

#define LED_PIN GPIO_PIN(PORT_A, 5) // assuming PORT_A and pin 5
#define TIMER_INTERVAL (1U * US_PER_SEC) // 1 second interval in microseconds

void timer_callback(void *arg) {
    gpio_set(LED_PIN);
}

int main(void) {
    // Initialize the LED pin
    gpio_init(LED_PIN, GPIO_OUT);
    
    // Setup the timer
    xtimer_ticks32_t interval = xtimer_usec_to_ticks32(TIMER_INTERVAL);
    xtimer_t timer;
    timer.callback = timer_callback;
    timer.arg = NULL;
    
    // Start timer - xtimer_set requires a pointer to the xtimer_t struct
    xtimer_set(&timer, interval);
    
    // Keep the main function alive, often done with idle loops or waiting for events
    while (1) {
        xtimer_sleep(1);
    }
    
    return 0;
}