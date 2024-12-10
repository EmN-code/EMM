#include <ztimer.h>
#include <stdlib.h>
#include <stdio.h>
#include <dht.h>
#include <periph/gpio.h>
#include <xtimer.h>

#define DHT_PIN GPIO_PIN(0, 2) // GPIO 2
#define INTERVAL (5U * US_PER_SEC) // 5 seconds

int main(void) {
    dht_t dev;
    int16_t temperature, humidity;

    // Initialize DHT11 sensor
    if (dht_init(&dev, DHT_TYPE_DHT11, DHT_PIN) != DHT_OK) {
        printf("Failed to initialize DHT11\n");
        return 1;
    }

    while (1) {
        // Read values from the sensor
        if (dht_read(&dev, &temperature, &humidity) == DHT_OK) {
            printf("Temp: %d.%dC, Humidity: %d.%d%%\n",
                    temperature / 10, temperature % 10,
                    humidity / 10, humidity % 10);
        } else {
            printf("Failed to read from DHT11\n");
        }

        // Wait 5 seconds
        ztimer_sleep(ZTIMER_MSEC, INTERVAL);
    }
    return 0;
}