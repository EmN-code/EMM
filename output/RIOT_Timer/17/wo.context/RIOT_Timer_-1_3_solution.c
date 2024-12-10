#include <stdio.h>
#include <xtimer.h>

#define LED_PIN 2
#define TIMER_INTERVAL (1000000U) // 1 second in microseconds

static void toggle_led(void *arg) {
    (void)arg;
    gpio_toggle(LED_PIN);
    printf("LED toggled\n");
}

int main(void) {
    // Initialize the LED pin
    gpio_init(LED_PIN, GPIO_OUT);
    
    // Initialize the timer
    xtimer_t timer;
    timer.callback = toggle_led;
    timer.arg = NULL;
    
    // Set the timer to trigger every second
    xtimer_set(&timer, TIMER_INTERVAL);
    
    // Loop forever
    while (1) {
        // Let the CPU sleep to save power
        xtimer_sleep(1);
    }
    return 0;
}