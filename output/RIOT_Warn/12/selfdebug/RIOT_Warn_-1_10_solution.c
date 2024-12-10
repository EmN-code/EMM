#include "periph/gpio.h"
#include "xtimer.h"
#include "dht.h"
#include <stdio.h>

#define DHT_PIN GPIO_PIN(PORT_A, 5)
#define LED_PIN GPIO_PIN(PORT_A, 6)
#define TEMP_THRESHOLD 30  // Temperature threshold in Celsius

int main(void) {
    // Initialize the DHT sensor and LED pin
    gpio_init(LED_PIN, GPIO_OUT);
    dht_t dev;
    if (dht_init(&dev, DHT_PIN, DHT_TYPE_DHT11) != DHT_OK) {
        printf("DHT initialization failed\n");
        return 1;
    }

    while (1) {
        // Read temperature and humidity
        int16_t temp, hum;
        if (dht_read(&dev, &temp, &hum) != DHT_OK) {
            printf("Failed to read from DHT sensor\n");
        } else {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temp / 10, temp % 10, hum / 10, hum % 10);

            // Turn on the LED if the temperature exceeds the threshold
            if ((temp / 10) > TEMP_THRESHOLD) {
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