
#include <stdio.h>
#include "xtimer.h"
#include "periph/gpio.h"
#include "dht.h"

#define DHT_PIN GPIO_PIN(0, 2)
#define INTERVAL (5 * 1000 * 1000U) /* 5 seconds in microseconds */

static void read_dht(void) {
    dht_t dev;
    int16_t temp, hum;

    if (dht_init(&dev, DHT_PIN) == DHT_OK) {
        if (dht_read(&dev, &temp, &hum) == DHT_OK) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
                   temp / 10, temp % 10, hum / 10, hum % 10);
        } else {
            puts("Error reading DHT sensor");
        }
    } else {
        puts("Error initializing DHT sensor");
    }
}

int main(void) {
    while (1) {
        read_dht();
        xtimer_usleep(INTERVAL);
    }

    return 0;
}
