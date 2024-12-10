#include <stdio.h>
#include <stdlib.h>
#include "xtimer.h"
#include "dht.h"

#define DHT_PIN 2
#define DHT_TYPE DHT11
#define SLEEP_TIME (5U * US_PER_SEC)

int main(void) {
    dht_t dev;
    int16_t temp, hum;

    if (dht_init(&dev, DHT_TYPE, DHT_PIN) != DHT_OK) {
        puts("Failed to initialize DHT11 sensor");
        return 1;
    }
    
    while (1) {
        if (dht_read(&dev, &temp, &hum) == DHT_OK) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%", temp / 10, abs(temp % 10), hum / 10, abs(hum % 10));
        } else {
            puts("Could not read data from DHT11 sensor");
        }
        
        xtimer_sleep(SLEEP_TIME);
    }
    return 0;
}