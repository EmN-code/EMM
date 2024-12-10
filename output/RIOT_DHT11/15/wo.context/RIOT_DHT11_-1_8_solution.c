#include <stdio.h> // Standard input/output library
#include <stdlib.h> // Standard library for functions like exit
#include <xtimer.h> // xtimer for delay
#include <periph/gpio.h> // GPIO peripheral library
#include <dht.h> // DHT sensor functions

#define DHT_PIN GPIO_PIN(0, 2) // Define the GPIO pin for DHT11

dht_t dev; // DHT device struct
dht_params_t params; // DHT parameters struct

int main(void) {
    // Initialize the DHT device
    params.pin = DHT_PIN;
    params.type = DHT11;

    int init_res = dht_init(&dev, &params);
    if (init_res != DHT_OK) {
        printf("Error: Could not initialize DHT11\n");
        return 1;
    }

    while (1) {
        int16_t temp, hum;
        int res = dht_read(&dev, &temp, &hum);
        if (res == DHT_OK) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temp / 10, temp % 10, hum / 10, hum % 10);
        } else {
            printf("Error: Could not read from sensor\n");
        }
        xtimer_sleep(5); // Sleep for 5 seconds
    }

    return 0;
}