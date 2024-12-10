#include <stdio.h>
#include "xtimer.h"
#include "periph/gpio.h"
#include "dht.h"

#define DHT_PIN GPIO_PIN(0, 4) // Example: GPIO pin 4
#define TEMP_THRESHOLD 30     // Temperature threshold for warning
#define WARNING_LED_PIN GPIO_PIN(0, 2) // Example: GPIO pin 2 as warning LED

int main(void) {
    // Initialize the DHT sensor
    if (dht_init(&dev, DHT_PIN, DHT11) != DHT_OK) {
        printf("Failed to initialize DHT11\n");
        return 1;
    }
    
    // Initialize the LED
    gpio_init(WARNING_LED_PIN, GPIO_OUT);
    
    int16_t temp, hum;
    
    // Loop to read the DHT11 every 5 seconds
    while (1) {
        // Read temperature and humidity
        if (dht_read(&dev, &temp, &hum) != DHT_OK) {
            printf("Failed to read from DHT11 sensor\n");
        } else {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temp / 10, temp % 10, hum / 10, hum % 10);

            // Check if temperature exceeds threshold
            if (temp / 10 > TEMP_THRESHOLD) {
                gpio_set(WARNING_LED_PIN); // Turn on LED
            } else {
                gpio_clear(WARNING_LED_PIN); // Turn off LED
            }
        }
        
        // Wait for 5 seconds
        xtimer_sleep(5);
    }
    return 0;
}