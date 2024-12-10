#include <stdio.h>
#include "xtimer.h"
#include "periph/gpio.h"

// Define GPIO pins for ESP32
#define DHT11_PIN GPIO_PIN(0, 4) // DHT11 data pin
#define LED_PIN GPIO_PIN(0, 2)   // LED pin

#define TEMP_THRESHOLD 30        // Temperature threshold for LED warning

void read_dht11(void) {
    // Dummy function to simulate reading DHT11 - replace with actual implementation
    int temp = 28; // Assume a temperature reading is obtained here

    // Check temperature and control LED
    if (temp > TEMP_THRESHOLD) {
        gpio_set(LED_PIN);
    } else {
        gpio_clear(LED_PIN);
    }
    printf("Temperature: %d\n", temp);
}

int main(void) {
    // Initialize GPIOs
    gpio_init(LED_PIN, GPIO_OUT);
    gpio_clear(LED_PIN); // Ensure LED is off initially

    // Main loop
    while (1) {
        read_dht11();
        xtimer_sleep(5); // Wait for 5 seconds
    }

    return 0;
}
