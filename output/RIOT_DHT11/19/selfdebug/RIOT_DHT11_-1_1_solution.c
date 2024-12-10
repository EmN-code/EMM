#include "thread_arch.h"
#include <stdio.h>
#include <stdint.h>
#include <xtimer.h>
#include "dht.h"

int main(void) {
    printf("Starting DHT11 sensor application...\n");

    dht_t sensor;
    dht_params_t params = {
        .pin = 2,  // Using GPIO2
        .type = DHT11
    };

    if (dht_init(&sensor, &params) != 0) {
        printf("Failed to initialize the DHT11 sensor\n");
        return 1;
    }

    while (1) {
        int16_t temp, hum;
        if (dht_read(&sensor, &temp, &hum) == 0) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temp/10, temp%10, hum/10, hum%10);
        } else {
            printf("Failed to read data from DHT11 sensor\n");
        }

        xtimer_sleep(5);  // Sleep for 5 seconds
    }

    return 0;
}