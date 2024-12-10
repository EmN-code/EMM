#include <stdio.h>
#include "xtimer.h"
#include "dht.h"
#include "periph/gpio.h"

#define DHT11_PIN GPIO_PIN(0, 2)  // GPIO 2
#define DHT11_TYPE DHT_TYPE_DHT11
#define SLEEP_INTERVAL (5U * 1000000U) // 5 seconds in microsecond

int main(void) {
    printf("DHT11 Sensor Reader on ESP32\n");

    dht_params_t dht_params;
    dht_params.pin = DHT11_PIN;
    dht_params.type = DHT11_TYPE;
    dht_t dev;

    if (dht_init(&dev, &dht_params) != DHT_OK) {
        printf("Error initializing DHT11\n");
        return 1;
    }

    while (1) {
        int16_t temperature, humidity;
        if (dht_read(&dev, &temperature, &humidity) != DHT_OK) {
            printf("Error reading DHT11\n");
        } else {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
                   temperature / 10, temperature % 10,
                   humidity / 10, humidity % 10);
        }

        xtimer_usleep(SLEEP_INTERVAL);
    }

    return 0;
}