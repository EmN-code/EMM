#include <xtimer.h>
#include <stdio.h>
#include <stdlib.h>
#include <periph/gpio.h>
#include "dht.h"

#define DHT11_PIN GPIO_PIN(0, 2) // Using GPIO2
#define INTERVAL (5U * US_PER_SEC) // Read every 5 seconds

int main(void) {
    dht_t dev;
    int16_t temperature, humidity;
    gpio_init(DHT11_PIN, GPIO_IN);

    // Initialize DHT11 sensor
    if (dht_init(&dev, DHT11_PIN, DHT_DHT11) != DHT_OK) {
        printf("Sensor initialization failed\n");
        return 1;
    }

    while (1) {
        // Read data
        if (dht_read(&dev, &temperature, &humidity) != DHT_OK) {
            printf("Failed to read from sensor\n");
        } else {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
                   temperature / 10, abs(temperature % 10),
                   humidity / 10, abs(humidity % 10));
        }
        xtimer_sleep(INTERVAL);
    }

    return 0;
}