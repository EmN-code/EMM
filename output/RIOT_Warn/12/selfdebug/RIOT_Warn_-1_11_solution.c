#include <xtimer.h>
#include <periph/gpio.h>
#include <stdlib.h>
#include "dht.h"

#define TEMP_THRESHOLD 30 // Temperature threshold to turn on the LED
#define DHT_PIN GPIO_PIN(0, 1) // Adjust according to your setup
#define LED_PIN GPIO_PIN(0, 2) // Adjust according to your setup

int main(void) {
    dht_t dev;

    // Initialize the DHT device
    if (dht_init(&dev, DHT_TYPE_DHT11) != 0) {
        printf("Failed to initialize DHT11\n");
        return 1;
    }

    // Set LED pin to output mode
    gpio_init(LED_PIN, GPIO_OUT);

    int16_t temperature, humidity;

    while (1) {
        // Read values from DHT11
        if (dht_read(&dev, &temperature, &humidity) != 0) {
            printf("Failed to read from DHT11\n");
        } else {
            // Convert temperature to degrees Celsius
            temperature /= 10;
            printf("Temperature: %d°C, Humidity: %d%%\n", temperature, humidity / 10);

            // Turn on LED if temperature exceeds threshold
            if (temperature > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);
            } else {
                gpio_clear(LED_PIN);
            }
        }

        // Wait for 5 seconds
        xtimer_sleep(5);
    }

    return 0;
}