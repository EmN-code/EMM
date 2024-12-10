#include <xtimer.h>  // Timer related functions
#include <thread_flags.h>
#include <periph/gpio.h>
#include <stdio.h>
#include "dht.h"  // Assuming dht.h provides the required function signatures

#define DHT_PIN GPIO_PIN(0, 2)
#define SLEEP_INTERVAL (5U * US_PER_SEC)

int main(void) {
    dht_t dev;
    
    // Initialize the DHT sensor
    if (dht_init(&dev, DHT_PIN) != DHT_OK) {
        printf("Error initializing DHT sensor\n");
        return 1;
    }

    while (1) {
        int16_t temp, hum;

        // Read values from the DHT sensor
        if (dht_read(&dev, &temp, &hum) != DHT_OK) {
            printf("Error reading from DHT sensor\n");
        } else {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temp / 10, temp % 10, hum / 10, hum % 10);
        }

        // Sleep for the defined interval
        xtimer_sleep(SLEEP_INTERVAL);
    }

    return 0;
}