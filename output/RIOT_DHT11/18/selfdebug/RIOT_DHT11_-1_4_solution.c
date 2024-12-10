#include <ztimer.h>  // ZTIMER_SEC is used for timing
#include <stdio.h>
#include "dht.h"
#include "dht_params.h"

// Define the DHT params here according to the sensor type and the GPIO pin used
dht_params_t dht_params = {
    .pin = GPIO2,  // DHT11 is connected to GPIO2
    .type = DHT11  // Specify the DHT type
};

int main(void) {
    int16_t temperature, humidity;
    dht_t dev;

    // Initialize the DHT11 sensor
    if (dht_init(&dev, &dht_params) != DHT_OK) {
        puts("DHT11 initialization failed!");
        return 1;
    }

    // Main loop
    while (1) {
        // Read values from the DHT11 sensor
        if (dht_read(&dev, &temperature, &humidity) == DHT_OK) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
                   temperature / 10, temperature % 10,
                   humidity / 10, humidity % 10);
        } else {
            puts("Failed to read from DHT11 sensor!");
        }

        // Wait for 5 seconds
        ztimer_sleep(ZTIMER_SEC, 5);
    }

    return 0;
}