#include <xtimer.h>
#include <stdio.h>
#include "periph/gpio.h"
#include "dht.h"

#define DHT_PIN         GPIO_PIN(PORT_A, 0)
#define LED_PIN         GPIO_PIN(PORT_B, 0)
#define TEMP_THRESHOLD  30  // Temperature threshold in Celsius
#define READ_INTERVAL   (5U * US_PER_SEC)

int main(void) {
    dht_t dev;
    int16_t temp, hum;

    // Initialize DHT sensor on PORT_A, pin 0
    if (dht_init(&dev, DHT11, DHT_PIN) != DHT_OK) {
        puts("Failed to initialize DHT11 sensor");
        return 1;
    }

    // Initialize LED on PORT_B, pin 0
    if (gpio_init(LED_PIN, GPIO_OUT)) {
        puts("Failed to initialize LED GPIO");
        return 1;
    }

    while (1) {
        // Read values from DHT11 sensor
        if (dht_read(&dev, &temp, &hum) != DHT_OK) {
            puts("Error reading DHT11 sensor");
            continue;
        }

        // Print the sensor values
        printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temp / 10, temp % 10, hum / 10, hum % 10);

        // Check if temperature exceeds threshold
        if (temp / 10 > TEMP_THRESHOLD) {
            gpio_set(LED_PIN);
        } else {
            gpio_clear(LED_PIN);
        }

        // Wait for the specified interval
        xtimer_sleep(READ_INTERVAL / US_PER_SEC);
    }
    return 0;
}