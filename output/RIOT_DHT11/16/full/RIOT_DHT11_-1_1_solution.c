#include <xtimer.h>
#include <stdio.h>
#include <dht.h>
#include "dht_params.h"

#define DHT_PIN 2
#define DHT_TYPE DHT11
#define INTERVAL (5U * US_PER_SEC)

int main(void) {
    dht_t dev;
    int16_t temp, hum;
    int rc;

    if ((rc = dht_init(&dev, &(dht_params_t){.pin = DHT_PIN, .type = DHT_TYPE})) != 0) {
        printf("DHT initialization failed with code %d\n", rc);
        return 1;
    }

    while (1) {
        if ((rc = dht_read(&dev, &temp, &hum)) != 0) {
            printf("DHT read failed with code %d\n", rc);
        } else {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temp / 10, temp % 10, hum / 10, hum % 10);
        }
        xtimer_sleep(INTERVAL);
    }

    return 0;
}