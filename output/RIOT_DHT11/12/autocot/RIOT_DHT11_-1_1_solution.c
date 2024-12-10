#include "thread_arch.h"
#include "thread_config.h"
#include "thread_flags.h"
#include "net/nanocoap/cache.h"
#include "event/timeout.h"
#include "event/periodic.h"
#include <stdio.h>
#include "dht11.h"
#include "xtimer.h"

// Configuration for DHT
#define DHT_PIN GPIO2
#define DHT_TYPE DHT11

// Delay between readings in microseconds (5 seconds)
#define READ_INTERVAL (5U * 1000000U)

void read_dht11(void) {
    int16_t temperature, humidity;
    dht_t dev;
    dht_params_t params;

    params.pin = DHT_PIN;
    params.type = DHT_TYPE;

    // Initialize the DHT device
    dht_init(&dev, &params);

    while (1) {
        // Read temperature and humidity
        if (dht_read(&dev, &temperature, &humidity) == 0) {
            printf("Temperature: %d, Humidity: %d\n", temperature, humidity);
        } else {
            printf("Failed to read from DHT11\n");
        }

        // Sleep for the READ_INTERVAL
        xtimer_usleep(READ_INTERVAL);
    }
}

int main(void) {
    // Start a new thread to read DHT11
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, read_dht11, NULL, "dht11_thread");

    return 0;
}