#include <stdio.h>
#include <string.h>
#include "xtimer.h"
#include "thread.h"
#include "net/emcute.h"
#include "lwip/ip6_addr.h"
#include "hashes/sha256.h"
#include "periph/gpio.h"
#include "dht.h"

#define DHT_PARAM_PIN GPIO_PIN(0, 2)
#define DHT_PARAM_TYPE DHT11
#define INTERVAL (5U * US_PER_SEC)

static dht_t dev;

int main(void) {
    int result;
    int16_t temperature, humidity;

    // Initialize the DHT sensor
    if (dht_init(&dev, DHT_PARAM_PIN, DHT_PARAM_TYPE) != DHT_OK) {
        printf("DHT initialization failed\n");
        return 1;
    }

    while (1) {
        // Read values from DHT
        result = dht_read(&dev, &temperature, &humidity);
        if (result == DHT_OK) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
                    temperature / 10, temperature % 10,
                    humidity / 10, humidity % 10);
        } else {
            printf("Error reading from DHT sensor\n");
        }

        // Sleep for the defined interval
        xtimer_usleep(INTERVAL);
    }
    return 0;
}