#include <stdio.h>
#include "xtimer.h"
#include "dht.h"
#include "periph/gpio.h"

#define DHT_PIN         GPIO_PIN(0, 2)
#define INTERVAL        (5U * 1000000U) // 5 seconds in microseconds

int main(void) {
    dht_t dev;
    int16_t temp, hum;
    
    dht_params_t params;
    params.pin = DHT_PIN;
    params.type = DHT11;
    
    if (dht_init(&dev, &params) == DHT_OK) {
        puts("DHT11 initialized successfully");
    } else {
        puts("Failed to initialize DHT11");
        return 1;
    }
    
    while (1) {
        if (dht_read(&dev, &temp, &hum) == DHT_OK) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temp / 10, temp % 10, hum / 10, hum % 10);
        } else {
            puts("Failed to read from DHT11 sensor");
        }
        
        xtimer_usleep(INTERVAL);
    }
    
    return 0;
}