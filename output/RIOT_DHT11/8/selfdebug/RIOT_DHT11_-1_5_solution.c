#include <xtimer.h>
#include <stdio.h>
#include <dht.h>
#include "dht_params.h"

#define INTERVAL    (5U * US_PER_SEC) // 5 seconds interval

// DHT sensor configuration
static dht_t dev;
static dht_params_t params = {
    .type = DHT11,                // Corrected to use DHT11 directly if defined, for example, in dht_params.h
    .pin = GPIO_PIN(0, 2)         // GPIO pin 2
};

int main(void) {
    // Initialize the sensor
    if (dht_init(&dev, &params) != DHT_OK) {
        printf("Failed to initialize DHT11 sensor\n");
        return 1;
    }

    // Main loop
    while (1) {
        int16_t temp, hum;
        if (dht_read(&dev, &temp, &hum) == DHT_OK) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
                   temp / 10, temp % 10, hum / 10, hum % 10);
        }
        else {
            printf("Failed to read from DHT11 sensor\n");
        }
        
        // Sleep for the interval duration
        xtimer_sleep(INTERVAL);
    }

    return 0;
}