#include <xtimer.h> // For timing functions
#include <stdio.h>   // For printf
#include <stdlib.h>  // For general utilities
#include "dht.h"    // For DHT sensor operations
#include "periph/gpio.h" // For GPIO operations

// Define the pin where the DHT11 data line is connected
#define DHT_PIN GPIO_PIN(0, 5)  // Example GPIO pin
// Define the GPIO pin for the warning LED
#define LED_PIN GPIO_PIN(0, 6)  // Example GPIO pin

// Temperature threshold in Celsius
#define TEMP_THRESHOLD 30

static dht_t dev;

int main(void) {
    int16_t temperature, humidity;
    xtimer_ticks32_t last = xtimer_now();
    
    // Initialize the DHT sensor
    if (dht_init(&dev, DHT_PIN, DHT11) != DHT_OK) {
        printf("Error initializing DHT sensor\n");
        return 1;
    }
    // Initialize the LED as output
    gpio_init(LED_PIN, GPIO_OUT);

    while (1) {
        // Read values from the DHT11 sensor
        if (dht_read(&dev, &temperature, &humidity) != DHT_OK) {
            printf("Failed to read from DHT sensor\n");
        } else {
            printf("Temperature: %dC, Humidity: %d%%\n", temperature, humidity);
            // Check if the temperature exceeds the threshold
            if (temperature > TEMP_THRESHOLD) {
                // Turn on the LED
                gpio_set(LED_PIN);
            } else {
                // Turn off the LED
                gpio_clear(LED_PIN);
            }
        }

        // Wait for 5 seconds
        xtimer_periodic_wakeup(&last, 5 * US_PER_SEC);
    }

    return 0;
}