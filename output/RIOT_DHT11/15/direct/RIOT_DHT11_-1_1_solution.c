#include <stdio.h>
#include "xtimer.h"
#include "periph/gpio.h"
#include "dht.h"

#define SLEEP_INTERVAL (5LU * US_PER_SEC)  // Interval of 5 seconds
#define DHT_PIN GPIO_PIN(0, 2)  // GPIO pin 2

static dht_t dev;

int main(void)
{
    printf("DHT11 Temperature and Humidity Sensor Reading\n");

    // Initialize the DHT11 sensor
    if (dht_init(&dev, DHT11, DHT_PIN) != 0) {
        printf("Failed to initialize DHT11 sensor\n");
        return 1;
    }

    while (1) {
        int16_t temperature, humidity;

        // Read the DHT11 sensor
        if (dht_read(&dev, &temperature, &humidity) != 0) {
            printf("Failed to read from DHT11 sensor\n");
        } else {
            // The temperature is divided by 10 to get a float value
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
                   temperature / 10, temperature % 10,
                   humidity / 10, humidity % 10);
        }

        // Sleep for the defined interval
        xtimer_sleep(SLEEP_INTERVAL);
    }

    return 0;
}