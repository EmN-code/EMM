#include <xtimer.h>
#include <stdio.h>
#include <periph/gpio.h>

#define TIMER_INTERVAL (1U * US_PER_SEC) // every second
#define LED_PIN GPIO_PIN(PORT_A, 5) // Assuming LED is connected to PORT_A pin 5

void timer_callback(void *arg) {
    (void)arg;
    gpio_toggle(LED_PIN);
}

int main(void) {
    // Initialize the LED pin as output
    if (gpio_init(LED_PIN, GPIO_OUT)) {
        puts("Error to initialize LED pin\n");
        return 1;
    }
    
    // Set up the timer
    xtimer_ticks32_t last_wakeup = xtimer_now();
    while (1) {
        xtimer_periodic_wakeup(&last_wakeup, TIMER_INTERVAL);
        timer_callback(NULL);
    }
    
    return 0;
}