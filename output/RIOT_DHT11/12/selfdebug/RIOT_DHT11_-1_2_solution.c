#include "xtimer.h"
#include "dht.h"
#include "periph/gpio.h"
#include <stdio.h>

#define DHT_PIN GPIO_PIN(0, 2)
#define INTERVAL (5U)

int main(void) {
    dht_t dev;
    int16_t temp, hum;
    xtimer_ticks32_t last_wakeup = xtimer_now();

    if (dht_init(&dev, DHT_TYPE_DHT11, DHT_PIN) != DHT_OK) {
        printf("Failed to initialize DHT11\n");
        return 1;
    }

    while (1) {
        if (dht_read(&dev, &temp, &hum) == DHT_OK) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temp / 10, temp % 10, hum / 10, hum % 10);
        } else {
            printf("Failed to read data from sensor\n");
        }
        xtimer_sleep(INTERVAL);
    }
    return 0;
}