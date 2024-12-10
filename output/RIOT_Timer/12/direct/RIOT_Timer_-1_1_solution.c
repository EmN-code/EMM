#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <xtimer.h>
#include <periph/gpio.h>

#define LED_PIN GPIO_PIN(PORT_B, 5)
#define INTERVAL 1000000U // 1 second in microseconds

void timer_callback(void *arg) {
    (void)arg;
    gpio_toggle(LED_PIN);
    printf("LED toggled!\n");
}

int main(void) {
    // Initialize the LED pin as an output
    if (gpio_init(LED_PIN, GPIO_OUT) < 0) {
        printf("Error: Unable to initialize the LED pin\n");
        return 1;
    }
    
    xtimer_ticks32_t last_wakeup = xtimer_now();
    while (1) {
        // Wait for the next tick
        xtimer_periodic_wakeup(&last_wakeup, INTERVAL);
        // Toggle LED in the callback
        timer_callback(NULL);
    }
    return 0;
}