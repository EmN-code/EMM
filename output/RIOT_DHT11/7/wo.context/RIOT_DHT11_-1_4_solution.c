#include <xtimer.h>
#include <stdio.h>
#include <periph/gpio.h>
#include "dht.h"

#define DHT11_GPIO_PIN GPIO_PIN(0, 2)
#define SLEEP_INTERVAL (5U * US_PER_SEC)

int main(void) {
    dht_t dev;
    int16_t temperature, humidity;

    /* Initialize the DHT sensor */
    if (dht_init(&dev, DHT11_GPIO_PIN) != DHT_OK) {
        printf("Failed to initialize DHT sensor\n");
        return 1;
    }

    while (1) {
        if (dht_read(&dev, &temperature, &humidity) != DHT_OK) {
            printf("Could not read data from DHT11 sensor\n");
        } else {
            printf("Temperature: %d.%d°C
", temperature / 10, temperature % 10);
            printf("Humidity: %d.%d%%
", humidity / 10, humidity % 10);
        }

        /* Sleep for 5 seconds */
        xtimer_sleep(SLEEP_INTERVAL / US_PER_SEC);
    }

    return 0;
}