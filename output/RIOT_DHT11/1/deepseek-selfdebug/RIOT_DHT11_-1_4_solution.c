
#include "xtimer.h"
#include "periph/gpio.h"
#include "dht.h"
#include <stdio.h>

#define DHT_PIN             GPIO_PIN(0, 2)
#define DHT_TYPE            DHT11
#define SLEEP_INTERVAL      (5 * 1000000U)  // 5 seconds

static dht_t dht_dev;

int main(void)
{
    int16_t temp, hum;

    // Initialize the DHT11 sensor
    dht_params_t dht_params = {
        .pin = DHT_PIN,
        .type = DHT_TYPE,
        .in_mode = GPIO_IN_PULLUP  // Corrected the undeclared error here
    };

    if (dht_init(&dht_dev, &dht_params) == DHT_OK) {
        printf("DHT11 sensor initialized successfully\n");
    } else {
        printf("Failed to initialize DHT11 sensor\n");
        return 1;
    }

    while (1) {
        if (dht_read(&dht_dev, &temp, &hum) == DHT_OK) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
                   temp / 10, temp % 10, hum / 10, hum % 10);
        } else {
            printf("Failed to read data from DHT11 sensor\n");
        }

        xtimer_usleep(SLEEP_INTERVAL);
    }

    return 0;
}
