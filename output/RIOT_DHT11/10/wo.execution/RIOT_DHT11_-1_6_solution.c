#include "ztimer.h"
#include "xtimer.h"
#include "stdlib.h"
#include "stddef.h"
#include "stdbool.h"
#include "dht.h"
#include "periph/gpio.h"
#include "stdio.h"

#define DHT11_PIN GPIO_PIN(0, 2)
#define READ_INTERVAL (5U * US_PER_SEC)

static dht_t dev;

int main(void) {
    printf("DHT11 sensor application initializing\n");

    if (dht_init(&dev, DHT11_PIN) != DHT_OK) {
        printf("Failed to initialize DHT11\n");
        return 1;
    }

    while (1) {
        int16_t temp, hum;
        if (dht_read(&dev, &temp, &hum) == DHT_OK) {
            printf("Temperature: %d.%d°C - Humidity: %d.%d%%\n",
                    temp / 10, temp % 10, hum / 10, hum % 10);
        } else {
            printf("Failed to read from DHT11\n");
        }

        ztimer_sleep(ZTIMER_SEC, 5);
    }

    return 0;
}