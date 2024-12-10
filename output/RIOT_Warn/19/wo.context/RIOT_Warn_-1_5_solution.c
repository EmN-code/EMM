#include <xtimer.h>
#include <stdlib.h>
#include <stdio.h>
#include <periph/gpio.h>
#include "dht.h"

#define DHT_PIN     GPIO_PIN(0, 0) // specify the actual pin connected to DHT
#define LED_PIN     GPIO_PIN(0, 1) // specify the actual pin connected to LED
#define TEMP_THRESHOLD 30 // temperature threshold in Celsius

int main(void) {
    // Initialize GPIO for LED
    if (gpio_init(LED_PIN, GPIO_OUT) < 0) {
        printf("Error initializing GPIO for LED\n");
        return 1;
    }

    // Initialize DHT
dht_params_t dht_params;
dht_params.pin = DHT_PIN;
dht_params.type = DHT_TYPE_DHT11;
    dht_t dev;
    dht_init(&dev, &dht_params);

    int16_t temp, hum;
    while (1) {
        // Read values from the DHT sensor
        if (dht_read(&dev, &temp, &hum) == 0) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temp / 10, temp % 10, hum / 10, hum % 10);

            // Check if temperature exceeds threshold
            if (temp / 10 > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);
                printf("Temperature threshold exceeded! LED turned on.\n");
            } else {
                gpio_clear(LED_PIN);
                printf("Temperature is normal. LED turned off.\n");
            }
        } else {
            printf("Failed to read data from sensor\n");
        }

        // Wait for 5 seconds
        xtimer_sleep(5);
    }
    return 0;
}
