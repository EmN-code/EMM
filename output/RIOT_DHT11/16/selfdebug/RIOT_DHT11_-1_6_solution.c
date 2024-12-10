#include <stdio.h>
#include <xtimer.h>
#include <periph/gpio.h>
#include "dht.h"

#define DHT_PIN GPIO_PIN(0, 2)
#define DHT_TYPE DHT11
#define INTERVAL (5U * US_PER_SEC)

dht_t dev;
int main(void) {
    // Initialize DHT device structure for DHT11 sensor on GPIO 2
    if (dht_init(&dev, DHT_PIN, DHT_TYPE) != DHT_OK) {
        printf("Failed to initialize DHT11
");
        return 1;
    }

    while (1) {
        int16_t temp, hum;
        if (dht_read(&dev, &temp, &hum) != DHT_OK) {
            printf("Error reading from DHT11
");
            continue;
        }

        printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", 
               temp / 10, temp % 10, hum / 10, hum % 10);

        xtimer_sleep(INTERVAL / US_PER_SEC);
    }
    return 0;
}