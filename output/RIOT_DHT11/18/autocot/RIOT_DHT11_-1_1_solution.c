#include <stdio.h>
#include <timex.h>
#include <xtimer.h>
// For DHT sensor
#include "dht.h"
#include "dht_params.h"

// DHT11 pin and type configurations
#define DHT11_PIN       GPIO_NUM_2
#define DHT11_TYPE      DHT11

// Define sensor and timer structs
static dht_t dev;

void read_dht(void) {
    int16_t temp, hum;
    if (dht_read(&dev, &temp, &hum) == 0) {
        // Print the temperature and humidity
        printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
                temp / 10, temp % 10, hum / 10, hum % 10);
    } else {
        printf("Error reading from DHT11 sensor\n");
    }
}

int main(void) {
    // Initialize the DHT sensor
    dht_params_t dht_params = {
        .pin = DHT11_PIN,
        .type = DHT11_TYPE
    };

    if(dht_init(&dev, &dht_params) != 0) {
        printf("Failed to initialize DHT11\n");
        return 1;
    }

    // Set a timer to call read_dht() every five seconds
    xtimer_ticks32_t last_wakeup = xtimer_now();
    while (1) {
        xtimer_periodic_wakeup(&last_wakeup, 5 * US_PER_SEC);
        read_dht();
    }

    return 0;
}