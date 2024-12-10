#include <xtimer.h>
#include <stdio.h>
#include "dht.h"

#define DHT_PIN GPIO_PIN(0, 2) // GPIO 2 for DHT11
#define DHT_TYPE DHT11
#define LOOP_INTERVAL (5U * 1000000U) // 5 seconds in microseconds

int main(void) {
    dht_t dev;
    dht_params_t params;
    int16_t temperature, humidity;
    int res;

    params.pin = DHT_PIN;
    params.type = DHT_TYPE;
    res = dht_init(&dev, &params);
    if (res != DHT_OK) {
        printf("Failed to initialize DHT sensor\n");
        return -1;
    }

    while (1) {
        res = dht_read(&dev, &temperature, &humidity);
        if ((res != DHT_OK)) {
            printf("Failed to read data from DHT sensor\n");
        } else {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", 
                   temperature / 10, temperature % 10, 
                   humidity / 10, humidity % 10);
        }
        xtimer_sleep(LOOP_INTERVAL / 1000000U);
    }
    return 0;
}