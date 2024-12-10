#include <xtimer.h>
#include <stdio.h>
#include "dht.h"
#include "periph/gpio.h"

#define DHT_PIN GPIO_PIN(0, 5) // Define the pin where the DHT11 data line is connected
#define LED_PIN GPIO_PIN(0, 15) // Define the pin where the LED is connected
#define TEMP_THRESHOLD 25 // Define temperature threshold for LED warning

static dht_t dev;

int main(void) {
    // Initialize the DHT device
    if (dht_init(&dev, DHT_PIN, DHT11) != 0) {
        printf("Failed to initialize DHT11\n");
        return 1;
    }

    // Initialize the LED pin
    gpio_init(LED_PIN, GPIO_OUT);

    while (1) {
        int16_t temp, hum;
        // Read the values from DHT11
        if (dht_read(&dev, &temp, &hum) == 0) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temp / 10, temp % 10, hum / 10, hum % 10);
            
            // Check if temperature exceeds threshold
            if (temp >= TEMP_THRESHOLD * 10) {
                // Turn on the LED
                gpio_set(LED_PIN);
            } else {
                // Turn off the LED
                gpio_clear(LED_PIN);
            }
        } else {
            printf("Failed to read from DHT11\n");
        }

        // Wait for 5 seconds
        xtimer_sleep(5);
    }

    return 0;
}