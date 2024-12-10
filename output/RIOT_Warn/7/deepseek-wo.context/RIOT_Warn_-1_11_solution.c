
#include "periph/gpio.h"
#include "periph/dht.h"
#include "xtimer.h"
#include <stdio.h>

#define DHT_PIN             GPIO_PIN(0, 12)  // GPIO12 on ESP32
#define LED_PIN             GPIO_PIN(0, 2)   // GPIO2 on ESP32
#define TEMP_THRESHOLD      30               // Temperature threshold in Celsius
#define INTERVAL_MS         5000000          // 5 seconds in microseconds

int main(void)
{
    dht_t dev;
    int16_t temp, hum;

    // Initialize the DHT11 sensor
    if (dht_init(&dev, DHT_PIN) == DHT_OK) {
        printf("DHT11 sensor initialized successfully\n");
    } else {
        printf("Failed to initialize DHT11 sensor\n");
        return 1;
    }

    // Initialize the LED pin as output
    gpio_init(LED_PIN, GPIO_OUT);
    gpio_set(LED_PIN);  // Turn off the LED initially

    while (1) {
        // Read temperature and humidity from the DHT11 sensor
        if (dht_read(&dev, &temp, &hum) == DHT_OK) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
                   temp / 10, temp % 10, hum / 10, hum % 10);

            // Check if the temperature exceeds the threshold
            if (temp / 10 > TEMP_THRESHOLD) {
                gpio_clear(LED_PIN);  // Turn on the LED
            } else {
                gpio_set(LED_PIN);    // Turn off the LED
            }
        } else {
            printf("Failed to read data from DHT11 sensor\n");
        }

        // Wait for 5 seconds
        xtimer_usleep(INTERVAL_MS);
    }

    return 0;
}
