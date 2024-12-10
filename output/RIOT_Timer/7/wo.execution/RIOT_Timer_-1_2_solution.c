#include <xtimer.h>
#include <stdio.h>
#include <periph/gpio.h>

#define LED_PIN GPIO_PIN(0, 2)  // Adjust based on your specific hardware

void timer_callback(void *arg) {
    (void)arg;
    gpio_toggle(LED_PIN);
    printf("LED toggled\n");
}

int main(void) {
    // Initialize the LED pin
    if (gpio_init(LED_PIN, GPIO_OUT)) {
        puts("Error initializing LED_PIN\n");
        return 1;
    }
    
    // Set the timer to trigger every second (1,000,000 microseconds)
    xtimer_ticks32_t interval = xtimer_us_to_ticks32(1000000);
    
    while (1) {
        xtimer_periodic_wakeup(&interval, interval);
        timer_callback(NULL);
    }
    
    return 0;
}