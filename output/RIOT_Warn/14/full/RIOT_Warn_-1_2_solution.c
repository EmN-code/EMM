// Description: This code initializes a DHT11 sensor and continuously reads temperature and humidity every 5 seconds. If the temperature exceeds a defined threshold, it turns on an LED as a warning.

#include "periph/gpio.h"
#include "event/thread.h"
#include "dht.h"
#include "xtimer.h"
#include <stdio.h>

#define DHT_PIN GPIO_PIN(0, 4) // Specifying the pin where the DHT11 is connected
#define LED_PIN GPIO_PIN(0, 2) // Specifying the pin where the LED is connected
#define TEMP_THRESHOLD 30 // Temperature threshold in degree Celsius

int main(void) {
    dht_t dev;
    int16_t temperature, humidity;
    gpio_init(LED_PIN, GPIO_OUT);

    // Initialize the DHT sensor
    while (dht_init(&dev, DHT_PIN, DHT11) != DHT_OK) {
        puts("DHT initialization failed. Retrying...");
        xtimer_sleep(1);
    }
    puts("DHT initialization successful.");

    while (1) {
        if (dht_read(&dev, &temperature, &humidity) == DHT_OK) {
            printf("Temperature: %d°C, Humidity: %d%%\n", temperature / 10, humidity / 10);

            if (temperature / 10 > TEMP_THRESHOLD) {
                gpio_set(LED_PIN); // Turn on the LED
                puts("Temperature threshold exceeded! LED turned on.");
            } else {
                gpio_clear(LED_PIN); // Turn off the LED
            }
        } else {
            puts("Failed to read from DHT sensor.");
        }

        xtimer_sleep(5); // Wait for 5 seconds before the next read
    }

    return 0;
}