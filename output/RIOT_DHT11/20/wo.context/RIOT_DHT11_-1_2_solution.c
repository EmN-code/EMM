#include <xtimer.h>
#include <thread_flags.h>
#include <stdio.h>
#include <dht.h>
#include <periph/gpio.h>

#define DHT_PIN GPIO_PIN(0, 2)
#define SLEEP_INTERVAL 5 

int main(void) {
    printf("DHT11 Reader on ESP32 Initialized\n");

    dht_t dev;
    int16_t temperature, humidity;

    // Corrected function call for dht_init
    if (dht_init(&dev, DHT11, DHT_PIN) != DHT_OK) {
        printf("Error initializing DHT11\n");
        return 1;
    }

    while (1) {
        if (dht_read(&dev, &temperature, &humidity) == DHT_OK) {
            // Convert and print temperature to Celsius
            printf("Temperature: %d.%dC\n", temperature / 10, temperature % 10);
            // Convert and print humidity
            printf("Humidity: %d.%d%%\n", humidity / 10, humidity % 10);
        } else {
            printf("Failed to read from DHT sensor\n");
        }

        // Sleep for the specified interval
        xtimer_sleep(SLEEP_INTERVAL);
    }

    return 0;
}