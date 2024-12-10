#include <xtimer.h>
#include <stdio.h>
#include "dht.h"
#include "periph/gpio.h"

#define DHT11_PIN GPIO_PIN(0, 2)
#define DELAY (5U * US_PER_SEC)

int main(void) {
    dht_t dev;
    int16_t temp, hum;

    dev.pin = DHT11_PIN;
    dev.type = DHT11;

    printf("DHT11 sensor reading on ESP32 example\n");

    if (dht_init(&dev) == 0) {
        puts("DHT initialization successful\n");
    } else {
        puts("DHT initialization failed\n");
        return -1;
    }

    while (1) {
        if (dht_read(&dev, &temp, &hum) == DHT_OK) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temp / 10, temp % 10, hum / 10, hum % 10);
        } else {
            puts("Failed to read data from sensor\n");
        }
        xtimer_usleep(DELAY);
    }
    return 0;
}