#include <thread.h>
#include <mutex.h>
#include <dht.h>
#include <xtimer.h>
#include <stdio.h>

// Define the DHT parameters
#define DHT_PIN GPIO_PIN(0, 2)
#define DHT_TYPE DHT11

int main(void) {
    printf("Starting DHT11 sensor reading task\n");

    dht_t dev;
    // Initialize the DHT sensor
    if (dht_init(&dev, DHT_TYPE, DHT_PIN) != DHT_OK) {
        printf("Error: Unable to initialize DHT11\n");
        return 1;
    }

    int16_t temperature, humidity;

    while (1) {
        // Read the sensor data
        if (dht_read(&dev, &temperature, &humidity) != DHT_OK) {
            printf("Error: Failed to read from DHT11\n");
        } else {
            printf("Temperature: %2.1f°C, Humidity: %2.1f%%\n", temperature / 10.0, humidity / 10.0);
        }

        // Wait for 5 seconds
        xtimer_sleep(5);
    }
    return 0;
}