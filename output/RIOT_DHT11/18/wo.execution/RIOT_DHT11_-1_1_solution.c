#include <xtensa/config/core.h>
#include <stdio.h>
#include <stdlib.h>
#include "dht.h"
#include "xtimer.h"

#define DHT_PIN GPIO_NUM_2   // The GPIO pin for DHT11
#define DELAY 5              // Delay in seconds

int main(void) {
    dht_t dev;
    int16_t temperature, humidity;

    dht_params_t params;
    params.pin = DHT_PIN;
    params.type = DHT11;

    if (dht_init(&dev, &params) != 0) {
        printf("Failed to initialize DHT11\n");
        return 1;
    }

    while (1) {
        if (dht_read(&dev, &temperature, &humidity) != 0) {
            printf("Failed to read from DHT11\n");
        } else {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temperature / 10, temperature % 10, humidity / 10, humidity % 10);
        }

        xtimer_sleep(DELAY);
    }

    return 0;
}