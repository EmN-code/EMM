#include <stdio.h>
#include <stdlib.h>
#include <xtimer.h>
#include <periph/gpio.h>
#include "dht.h"

#define SLEEP_TIME (5U * US_PER_SEC)
#define DHT_DATA_PIN (GPIO_PIN(0, 2))  // using GPIO 2

int main(void) {
    dht_t dev;
    dht_params_t params;
    params.pin = DHT_DATA_PIN;
    params.type = DHT11;
    
    if (dht_init(&dev, &params) != DHT_OK) {
        printf("Failed to initialize DHT sensor\n");
        return 1;
    }

    while (1) {
        int16_t temp, hum;
        if (dht_read(&dev, &temp, &hum) == DHT_OK) {
            printf("Temperature: %d.%d°C - Humidity: %d.%d%%\n", 
                   temp / 10, abs(temp % 10), 
                   hum / 10, abs(hum % 10));
        }
        else {
            printf("Failed to read data from DHT sensor\n");
        }
        xtimer_sleep(SLEEP_TIME);
    }

    return 0;
}