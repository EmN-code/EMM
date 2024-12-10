#include "xtimer.h"
#include "dht.h"
#include "dht_params.h"
#include <stdio.h>

#define DHT_TYPE DHT11
#define DHT_PIN GPIO_PIN(0, 2)
#define INTERVAL (5U * US_PER_SEC)

int main(void) {
    dht_t dev;
    int16_t temp, hum;
    int rc;

    dht_params_t params;
    params.pin = DHT_PIN;
    params.type = DHT_TYPE;

    rc = dht_init(&dev, &params);
    if (rc != 0) {
        printf("Failed to initialize DHT sensor\n");
        return 1;
    }

    while(1) {
        rc = dht_read(&dev, &temp, &hum);
        if (rc != 0) {
            printf("DHT read failed\n");
        } else {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temp / 10, temp % 10, hum / 10, hum % 10);
        }
        xtimer_sleep(5);
    }

    return 0;
}