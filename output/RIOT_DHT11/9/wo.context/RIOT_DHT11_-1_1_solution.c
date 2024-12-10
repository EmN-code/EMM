#include <stdio.h>
#include <time.h>
#include <thread_arch.h>
#include <thread_flags.h>
#include "dht.h"

// DHT device and parameters
static dht_t dht_dev;
static const dht_params_t dht_params = {
    .pin = 2,            // GPIO 2
    .type = DHT11        // DHT11 sensor
};

void read_dht(void) {
    int16_t temperature, humidity;
    // Initialize the DHT sensor
    if (dht_init(&dht_dev, &dht_params) == DHT_OK) {
        printf("DHT11 initialized on GPIO 2.\n");
    } else {
        printf("Failed to initialize DHT11.\n");
        return;
    }

    while (1) {
        // Read temperature and humidity
        if (dht_read(&dht_dev, &temperature, &humidity) == DHT_OK) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", 
                   temperature / 10, temperature % 10, 
                   humidity / 10, humidity % 10);
        } else {
            printf("Failed to read data from DHT11.\n");
        }

        // Sleep for 5 seconds
        thread_sleep(5000000);
    }
}

int main(void) {
    printf("Starting DHT11 read on ESP32 with GPIO 2\n");
    // Start DHT reading in a separate thread
    thread_create(NULL, 512, THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, read_dht, NULL, "dht_reader");

    while (1) {
        // Main loop can be used to perform other tasks
        thread_yield();   // Yield the CPU
    }

    return 0;
}