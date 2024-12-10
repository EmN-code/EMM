#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "xtimer.h"
#include "periph/gpio.h"
#include "dht.h"

#define DHT_PIN         GPIO_PIN(PORT_A, 0)  // Use a valid GPIO port
#define LED_PIN         GPIO_PIN(PORT_B, 0)  // Use a valid GPIO port
#define TEMP_THRESHOLD  30  // Temperature threshold in degree Celsius
#define DELAY           5    // Delay in seconds

int main(void) {
    // Initialize DHT sensor
    dht_t dev;
    if (dht_init(&dev, DHT_PIN, DHT11) != DHT_OK) {
        printf("Failed to initialize DHT11\n");
        return 1;
    }

    // Initialize LED pin
    gpio_init(LED_PIN, GPIO_OUT);

    int16_t temp, hum;

    while (1) {
        // Read data from DHT sensor
        if (dht_read(&dev, &temp, &hum) != DHT_OK) {
            printf("Failed to read from DHT11\n");
        } else {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
                temp / 10, temp % 10, hum / 10, hum % 10);

            // Check if temperature exceeds threshold
            if (temp / 10 > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);
                printf("Temperature threshold exceeded!\n");
            } else {
                gpio_clear(LED_PIN);
            }
        }
        xtimer_sleep(DELAY);
    }
    return 0;
}