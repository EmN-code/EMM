#include "xtimer.h"
#include "dht.h"
#include "gpio.h"
#include <stdio.h>

#define DHT_PIN GPIO_PIN(0, 2)

int main(void) {
    printf("Starting DHT11 reader on GPIO 2\n");
    dht_t dev;
    int16_t temperature, humidity;

    if (dht_init(&dev, DHT_PIN) != DHT_OK) {
        printf("Error initializing DHT11\n");
        return 1;
    }

    while (1) {
        if (dht_read(&dev, &temperature, &humidity) == DHT_OK) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", 
                   temperature / 10, temperature % 10, 
                   humidity / 10, humidity % 10);
        } else {
            printf("Error reading DHT11\n");
        }

        xtimer_sleep(5);
    }

    return 0;
}