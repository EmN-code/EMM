// Description
// This code initializes the necessary components for a basic RIOT application on an ESP32. 
// It reads data from a DHT11 sensor every 5 seconds and turns on an LED if the temperature exceeds a predefined threshold.
// Make sure to define PORT_B and GPIO pin numbers correctly as per your hardware setup.

// Imports
#include <xtimer.h>
#include <periph/gpio.h>
#include "dht.h"

#define DHT11_PIN GPIO_PIN(PORT_A, 2) // Set up the correct GPIO port
#define LED_PIN GPIO_PIN(PORT_A, 1)   // Set up the correct GPIO port
#define TEMP_THRESHOLD 30             // Temperature threshold for LED warning

int main(void) {
    // Initialize the DHT11 sensor
    dht_t dev;
    int16_t temp, hum;

    // Configure DHT11 pin
    gpio_init(DHT11_PIN, GPIO_IN);
    // Configure LED pin
    gpio_init(LED_PIN, GPIO_OUT);

    while (1) {
        // Read temperature and humidity data
        if (dht_read(&dev, &temp, &hum) == 0) {
            // Check if temperature exceeds threshold
            if (temp > TEMP_THRESHOLD) {
                // Turn on LED
                gpio_set(LED_PIN);
            } else {
                // Turn off LED
                gpio_clear(LED_PIN);
            }
        }
        // Wait for 5 seconds
        xtimer_sleep(5);
    }
    return 0;
}