#include <xtimer.h>
#include <stdio.h>
#include "dht.h"
#include "periph/gpio.h"

#define DHT_PIN GPIO_PIN(0, 2)
#define SLEEP_INTERVAL (5U * US_PER_SEC) // 5 seconds

dht_t dev;

int main(void) {
    if (dht_init(&dev, DHT_TYPE_DHT11, DHT_PIN) != 0) {
        printf("Error initializing DHT11\n");
        return 1;
    }

    while (1) {
        int16_t temp, hum;
        if (dht_read(&dev, &temp, &hum) != DHT_OK) {
            printf("Error reading DHT11\n");
        } else {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temp / 10, temp % 10, hum / 10, hum % 10);
        }
        xtimer_sleep(SLEEP_INTERVAL);
    }

    return 0;
}