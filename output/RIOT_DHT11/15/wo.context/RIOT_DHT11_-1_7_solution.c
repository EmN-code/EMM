#include "xtimer.h"
#include "periph/gpio.h"
#include "dht.h"
#include <stdio.h>

#define DHT_PIN GPIO_PIN(0, 2)
#define DHT_TYPE DHT11

int main(void)
{
    dht_t dev;
    int16_t temp, hum;

    if (dht_init(&dev, DHT_PIN, DHT_TYPE) != 0) {
        printf("Failed to initialize DHT sensor\n");
        return 1;
    }

    while (1) {
        if (dht_read(&dev, &temp, &hum) != 0) {
            printf("Failed to read from DHT sensor\n");
        }
        else {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", 
                   temp / 10, temp % 10, hum / 10, hum % 10);
        }
        xtimer_sleep(5);
    }

    return 0;
}