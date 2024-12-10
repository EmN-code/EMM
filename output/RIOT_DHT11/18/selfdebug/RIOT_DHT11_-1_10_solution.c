#include <stdio.h>
#include <string.h>
#include "xtimer.h"
#include "timex.h"
#include "periph/gpio.h"
#include "dht.h"

#define DHT_PIN GPIO_PIN(0, 2)  // Define GPIO pin for DHT11
#define INTERVAL (5U * US_PER_SEC)  // Read interval of 5 seconds

static dht_t dht_dev;

static void read_dht(void) {
    int16_t temp, hum;
    if (dht_read(&dht_dev, &temp, &hum) == DHT_OK) {
        printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
               temp / 10, temp % 10, hum / 10, hum % 10);
    } else {
        printf("Failed to read from DHT11 sensor\n");
    }
}

int main(void) {
    if (dht_init(&dht_dev, DHT11, DHT_PIN) != DHT_OK) {
        printf("Error initializing DHT11\n");
        return 1;
    }

    printf("Starting DHT11 sensor read every 5 seconds\n");

    while (1) {
        read_dht();
        xtimer_sleep(INTERVAL / US_PER_SEC);
    }

    return 0;
}