#include <ztimer.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <dht.h>
#include <dht_params.h>

#define DHT11_PIN GPIO_PIN(0, 2) // GPIO pin 2
#define SLEEP_INTERVAL (5U * 1000000U) // 5 seconds in microseconds

int main(void) {
    dht_t dev;
    int res;
    int16_t temp, hum;

    // Initialize the DHT11 sensor
    dev.params = dht_params[DHT11_PIN];
    res = dht_init(&dev, &dht_params[DHT11_PIN]);
    while (1) {
        // Attempt to read from the sensor
        res = dht_read(&dev, &temp, &hum);
        if (res == DHT_OK) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temp / 10, temp % 10, hum / 10, hum % 10);
        } else {
            printf("Failed to read from DHT11 sensor, retrying...\n");
        }
        
        // Wait for 5 seconds before reading again
        ztimer_sleep(ZTIMER_USEC, SLEEP_INTERVAL);
    }

    return 0;
}