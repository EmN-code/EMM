#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "xtimer.h"
#include "thread.h"
#include "dht.h"
#include "periph/gpio.h"

#define DHT_PIN GPIO_PIN(0, 2)  // GPIO pin for DHT11
#define SLEEP_TIME 5  // Sleep time in seconds

int main(void) {
    dht_t dev;
    int16_t temp, hum;

    if (dht_init(&dev, DHT11, DHT_PIN) != DHT_OK) {
        puts("Error initializing DHT11 sensor");
        return 1;
    }

    while (1) {
        if (dht_read(&dev, &temp, &hum) == DHT_OK) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temp / 10, temp % 10, hum / 10, hum % 10);
        } else {
            puts("Failed to read from DHT11 sensor");
        }

        xtimer_sleep(SLEEP_TIME);
    }

    return 0;
}