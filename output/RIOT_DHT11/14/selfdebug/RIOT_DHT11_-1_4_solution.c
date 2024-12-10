#include <stdio.h> // Required for printf function
#include <stdint.h> // Required for int16_t type
#include "xtimer.h" // Timer functionality for scheduling
#include "periph/gpio.h" // GPIO functionalities
#include "dht.h" // DHT sensor library

int main(void) {
    dht_t sensor;
    gpio_t pin = GPIO_PIN(0, 2); // assuming 0 is the port and 2 is the pin
    int16_t temp, hum;
    int result;

    // Initialize the DHT sensor
    // Please note, directly using DHT_TYPE_DHT11 is avoided as it's not defined, using '0' typically indicates DHT11
    dht_params_t params = { .pin = pin, .type = 0 };
    if ((result = dht_init(&sensor, &params)) != DHT_OK) {
        printf("Error initializing DHT sensor\n");
        return result;
    }

    while (1) {
        // Read values from the DHT sensor
        if ((result = dht_read(&sensor, &temp, &hum)) == DHT_OK) {
            printf("Temperature: %d.%d C, Humidity: %d.%d %%\n", 
                   temp / 10, temp % 10, hum / 10, hum % 10);
        } else {
            printf("Error reading DHT sensor\n");
        }
        
        // Wait for 5 seconds
        xtimer_sleep(5);
    }
    return 0;
}