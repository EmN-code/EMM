#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>

#include "xtimer.h"
#include "periph/gpio.h"
#include "dht.h"

#define DHT_GPIO_PIN  GPIO_PIN(0, 2)  // GPIO pin 2
#define SLEEP_INTERVAL   (5U * US_PER_SEC)

// Assuming the respective header or value for DHT_TYPE_DHT11
#define DHT_TYPE_DHT11     0

int main(void) {
    dht_t dev;
    int16_t temp, hum;
    
    // Initialize the DHT sensor
    if (dht_init(&dev, DHT_GPIO_PIN, DHT_TYPE_DHT11) != DHT_OK) {
        printf("Failed to initialize DHT sensor\n");
        return 1;
    }

    while (1) {
        // Read the values from the DHT11 sensor
        if (dht_read(&dev, &temp, &hum) == DHT_OK) {
            printf("Temperature: %d.%dC, Humidity: %d.%d%%\n", 
                   temp / 10, abs(temp % 10), 
                   hum / 10, hum % 10);
        } else {
            printf("Failed to read from DHT sensor\n");
        }

        // Wait for the defined interval
        xtimer_sleep(SLEEP_INTERVAL / US_PER_SEC);
    }

    return 0;
}