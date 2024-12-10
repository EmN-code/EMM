#include "periph/gpio.h"
#include "xtimer.h"
#include "dht.h"
#include <stdio.h>

#define DHT_PIN GPIO_PIN(0, 2) // GPIO2
#define INTERVAL (5U * US_PER_SEC) // 5 seconds

int main(void) {
    dht_t dev;
    int16_t temp, hum;

    if (dht_init(&dev, DHT_TYPE_DHT11, DHT_PIN) != DHT_OK) {
        puts("Failed to initialize DHT11 sensor");
        return 1;
    }

    while (1) {
        if (dht_read(&dev, &temp, &hum) != DHT_OK) {
            puts("Failed to read data from DHT11 sensor");
        }
        else {
            printf("Temperature: %d.%d°C
", temp / 10, temp % 10);
            printf("Humidity: %d.%d%%
", hum / 10, hum % 10);
        }
        xtimer_sleep(5);
    }
    return 0;
}