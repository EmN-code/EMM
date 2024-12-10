#include <xtimer.h>
#include <stdio.h>
#include <stdlib.h>
#include <periph/gpio.h>
#include <dht.h>

#define DHT_PIN GPIO_PIN(0, 2)
#define DHT_TYPE DHT11
#define INTERVAL (5U * US_PER_SEC)  // 5 seconds

int main(void) {
    dht_t dev;
    if (dht_init(&dev, DHT_TYPE, DHT_PIN) != DHT_OK) {
        printf("Failed to initialize DHT sensor\n");
        return 1;
    }

    while (1) {
        int16_t temp, hum;
        if (dht_read(&dev, &temp, &hum) == DHT_OK) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
                   temp / 10, temp % 10, hum / 10, hum % 10);
        }
        else {
            printf("Failed to read from DHT sensor\n");
        }
        xtimer_sleep(INTERVAL);
    }
    return 0;
}