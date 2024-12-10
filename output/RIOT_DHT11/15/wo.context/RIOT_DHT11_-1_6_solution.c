#include "xtimer.h"
#include "thread.h"
#include "periph/gpio.h"
#include "dht.h"
#include <stdio.h>

#define DHT_PIN GPIO_PIN(0, 2)

static dht_t dev;

int main(void) {
    int16_t temperature, humidity;
    while (1) {
        if (dht_init(&dev, DHT_MODEL_DHT11) != DHT_OK) {
            printf("Failed to initialize DHT11\n");
            xtimer_sleep(1);
            continue;
        } else {
            if (dht_read(&dev, &temperature, &humidity) == DHT_OK) {
                printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temperature / 10, temperature % 10, humidity / 10, humidity % 10);
            } else {
                printf("Failed to read data from DHT11\n");
            }
        }
        xtimer_sleep(5);
    }
    return 0;
}