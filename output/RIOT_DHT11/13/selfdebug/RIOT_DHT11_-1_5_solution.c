#include <stdio.h>
#include <xtimer.h>
#include <periph/gpio.h>
#include "dht.h"

#define DHT_PIN GPIO_PIN(0, 2) // GPIO 2 is equivalent to pin 0_2
#define INTERVAL (5U * US_PER_SEC)

int main(void)
{
    dht_t dev;
    int16_t temperature, humidity;

    if (dht_init(&dev, DHT_PIN, DHT11) != DHT_OK) {
        printf("DHT initialization failed\n");
        return 1;
    }

    while (1) {
        if (dht_read(&dev, &temperature, &humidity) == DHT_OK) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
                   temperature / 10, temperature % 10,
                   humidity / 10, humidity % 10);
        } else {
            printf("Failed to read DHT sensor\n");
        }
        xtimer_sleep(INTERVAL);
    }

    return 0;
}