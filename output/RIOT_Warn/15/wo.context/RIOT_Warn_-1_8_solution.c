#include <xtimer.h>  // Timer related functions
#include <stdio.h>   // Standard input/output functions
#include <periph/gpio.h>  // GPIO related functions
#include "dht.h"  // Include DHT sensor library

#define DHT_PIN GPIO_PIN(0, 4)  // Define the pin where the DHT sensor is connected
#define LED_PIN GPIO_PIN(0, 2)  // Define the pin where the LED is connected
#define TEMP_THRESHOLD 30  // Temperature threshold to toggle LED

int main(void) {
    gpio_init(LED_PIN, GPIO_OUT);  // Initialize LED pin as output
    gpio_clear(LED_PIN);  // Ensure LED is off initially

    dht_t dev;
    int16_t temperature, humidity;

    // Initialize DHT sensor
    if (dht_init(&dev, DHT_PIN, DHT11) != DHT_OK) {
        printf("Could not initialize DHT sensor\n");
        return 1;
    }

    while (1) {
        if (dht_read(&dev, &temperature, &humidity) == DHT_OK) {
            printf("Temperature: %d°C, Humidity: %d%%\n", temperature, humidity);

            // Check if the temperature exceeds the threshold
            if (temperature > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);  // Turn on the LED
            } else {
                gpio_clear(LED_PIN);  // Turn off the LED
            }
        } else {
            printf("Failed to read from DHT sensor\n");
        }

        xtimer_sleep(5);  // Wait for 5 seconds before reading again
    }

    return 0;
}