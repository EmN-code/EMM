
#include "xtimer.h"
#include "periph/gpio.h"
#include "dht.h"
#include <stdio.h>

#define DHT_PIN GPIO_PIN(0, 2)
#define INTERVAL (5 * 1000 * 1000U) /* 5 seconds in microseconds */

static void read_dht(void) {
    dht_t dev;
    int16_t temp, hum;

    if (dht_init(&dev, DHT_PIN) != DHT_OK) {
        puts("Error initializing DHT sensor");
        return;
    }

    if (dht_read(&dev, &temp, &hum) != DHT_OK) {
        puts("Error reading from DHT sensor");
        return;
    }

    printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
           temp / 10, temp % 10, hum / 10, hum % 10);
}

int main(void) {
    xtimer_ticks32_t last_wakeup = xtimer_now();

    while (1) {
        read_dht();
        xtimer_periodic_wakeup(&last_wakeup, INTERVAL);
    }

    return 0;
}
