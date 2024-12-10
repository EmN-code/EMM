#include <xtimer.h>
#include <stdio.h>
#include <periph/gpio.h>

// Assuming the DHT library is correctly included, we need to use the correct initialization function:
// #include "dht.h"  // Uncomment this line if there is a specific library for handling DHT sensors in RIOT.

#define DHT_GPIO_PIN GPIO_PIN(0, 2)  // Defines GPIO 0, pin 2.
#define INTERVAL 5

int main(void) {
    // If a specific init function exists, we use that:
    dht_params_t dht_params;
    dht_params.pin = DHT_GPIO_PIN;
    dht_params.type = DHT11;

    // Assume there is some kind of dht-temp initialization, modify if needed to match actual function.
    if (dht_init(&dht_params) != 0) {
        printf("Failed to initialize the DHT11 sensor\n");
        return 1;
    }

    while (1) {
        int16_t temp, hum;
        if (dht_read(&dht_params, &temp, &hum) == 0) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temp/10, temp%10, hum/10, hum%10);
        } else {
            printf("Error reading from DHT11 sensor\n");
        }
        xtimer_sleep(INTERVAL);
    }
    return 0;
}