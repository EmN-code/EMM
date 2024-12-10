#include <stdio.h>
#include <time.h>
#include "periph/gpio.h"
#include "dht.h"
#include "xtimer.h"

#define DHT_PIN GPIO_PIN(0, 2) // GPIO02 for ESP32
#define DHT_TYPE DHT11

int main(void)
{
    dht_t dev;
    int16_t temperature, humidity;
    int res;

    // Initialize the DHT sensor
    if (dht_init(&dev, DHT_TYPE, DHT_PIN) != DHT_OK) {
        printf("Failed to initialize DHT sensor\n");
        return 1;
    }

    while (1) {
        // Read temperature and humidity data
        res = dht_read(&dev, &temperature, &humidity);
        if (res == DHT_OK) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", 
                    temperature / 10, temperature % 10, 
                    humidity / 10, humidity % 10);
        } else {
            printf("Failed to read from DHT sensor\n");
        }

        // Wait for 5 seconds
        xtimer_sleep(5);
    }
}
