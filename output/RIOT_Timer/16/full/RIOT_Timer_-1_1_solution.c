#include "xtimer.h"
#include "periph/gpio.h"

#define LED_PIN GPIO_PIN(0, 2)  // Use GPIO pin 2 (adjust based on actual setup)

void timer_handler(void *arg) {
    (void)arg;
    
    // Toggle LED state
    gpio_toggle(LED_PIN);
}

int main(void) {
    // Initialize the LED_PIN as an output
    if (gpio_init(LED_PIN, GPIO_OUT)) {
        printf("Error: unable to initialize LED GPIO pin\n");
        return 1;
    }

    // Infinite loop to toggle LED every second
    while (1) {
        timer_handler(NULL);
        xtimer_sleep(1);  // Wait for one second
    }

    return 0;
}