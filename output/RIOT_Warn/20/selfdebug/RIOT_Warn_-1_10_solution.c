#include "xtimer.h"
#include "stdio.h"
#include "periph/gpio.h"
#include "dht.h"

#define DHT_PIN GPIO_PIN(PA, 8)
#define LED_PIN GPIO_PIN(PA, 9)
#define TEMP_THRESHOLD 30

int main(void) {
    dht_t dev;

    // Initialize the DHT11 sensor
    if (dht_init(&dev, DHT_PIN) != DHT_OK) {
        puts("DHT sensor initialization failed!");
        return 1;
    }

    // Initialize the LED pin
    if (gpio_init(LED_PIN, GPIO_OUT) < 0) {
        puts("Error initializing LED pin\n");
        return 1;
    }

    while (1) {
        int16_t temp, hum;

        // Read values from DHT11 sensor
        if (dht_read(&dev, &temp, &hum) != DHT_OK) {
            puts("Failed to read from sensor\n");
        } else {
            printf("Temperature: %d°C, Humidity: %d%%\n", temp, hum);

            // Check if temperature exceeds threshold
            if (temp > TEMP_THRESHOLD) {
                // Turn on the LED
                gpio_set(LED_PIN);
            } else {
                // Turn off the LED
                gpio_clear(LED_PIN);
            }
        }

        // Wait for 5 seconds
        xtimer_sleep(5);
    }

    return 0;
}