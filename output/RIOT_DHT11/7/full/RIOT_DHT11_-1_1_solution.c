#include <xtimer.h>
#include <stdio.h>
#include <dht.h>
#include "periph/gpio.h"

#define DHT_PIN GPIO_PIN(PORT_A, 2)
#define DELAY 5

static dht_t dht_dev;

int main(void) {
    int16_t temperature, humidity;

    if (dht_init(&dht_dev, DHT_PIN) != 0) {
        printf("Failed to initialize DHT sensor\n");
        return 1;
    }

    while (1) {
        if (dht_read(&dht_dev, &temperature, &humidity) != 0) {
            printf("Error reading from DHT sensor\n");
        } else {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", 
                   temperature / 10, temperature % 10, 
                   humidity / 10, humidity % 10);
        }
        xtimer_sleep(DELAY);
    }
    return 0;
}