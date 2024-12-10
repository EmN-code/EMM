#include <thread.h>
#include <stdio.h>
#include <dht.h>
#include <xtimer.h>

// Constants
#define DHT_PIN DHT11_GPIO2
#define READ_INTERVAL (5U * US_PER_SEC) // 5 seconds in microseconds

// Global variable for the DHT device
static dht_t dht_dev;

// Thread function to read from DHT11 every 5 seconds
void *read_dht11(void *arg) {
    (void)arg;
    int16_t temp, hum;
    while (1) {
        // Read temperature and humidity
        if (dht_read(&dht_dev, &temp, &hum) == DHT_OK) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temp / 10, temp % 10, hum / 10, hum % 10);
        } else {
            printf("Failed to read from DHT11\n");
        }

        // Wait for next reading
        xtimer_sleep(5);
    }
    return NULL;
}

int main(void) {
    // Initialize the DHT device
    dht_params_t params;
    params.pin = DHT_PIN;
    params.type = DHT11;
    if (dht_init(&dht_dev, &params) != DHT_OK) {
        printf("DHT initialization failed\n");
        return 1;
    }

    // Create the DHT reading thread
    thread_create(NULL, THREAD_STACKSIZE_DEFAULT, THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, read_dht11, NULL, "dht thread");

    // Main thread can perform other tasks or sleep indefinitely
    while (1) {
        xtimer_sleep(60);
    }

    return 0;
}