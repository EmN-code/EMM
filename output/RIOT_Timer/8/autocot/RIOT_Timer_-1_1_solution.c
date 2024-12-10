#include <stdio.h>
#include "xtimer.h"
#include "periph/gpio.h"

#define LED_PIN GPIO_PIN(PORT_A, 5) // Assuming the LED is connected to PORT_A pin 5
#define INTERVAL (1U * US_PER_SEC)  // 1-second interval

int main(void) {
    // Initialize LED_PIN as output
    gpio_init(LED_PIN, GPIO_OUT);
    
    while (1) {
        // Set the LED_PIN high
        gpio_set(LED_PIN);
        // Wait for the interval duration
        xtimer_usleep(INTERVAL);
        // Set the LED_PIN low
        gpio_clear(LED_PIN);
        // Wait for the interval duration
        xtimer_usleep(INTERVAL);
    }
    return 0;
}