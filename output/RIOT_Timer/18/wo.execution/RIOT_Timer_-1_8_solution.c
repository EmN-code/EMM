#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <xtimer.h>
#include <periph/gpio.h>

#define BLINK_INTERVAL (1000000U) // 1 second interval in microseconds
#define LED_PIN (GPIO_PIN(PORT_B, 0))

int main(void) {
    // Initialize the LED_PIN as an output
    gpio_init(LED_PIN, GPIO_OUT);
    
    printf("Starting LED blink example on ESP32 using RIOT\n");
    
    while (1) {
        // Turn the LED on
        gpio_set(LED_PIN);
        
        // Sleep for the blink interval
        xtimer_usleep(BLINK_INTERVAL);
        
        // Turn the LED off
        gpio_clear(LED_PIN);
        
        // Sleep for the blink interval
        xtimer_usleep(BLINK_INTERVAL);
    }
    return 0;
}