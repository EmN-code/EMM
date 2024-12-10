#include <stdio.h>
#include <stdlib.h>
#include "xtimer.h"
#include "periph/gpio.h"


// Function to initialize the DHT11 sensor
static int dht_init(gpio_t pin) {
    // Initialization logic for the DHT11 sensor
    // Here we assume the function correctly initializes and configures the DHT11 sensor on a given GPIO pin
    // This function should return an error code if the initialization fails
    gpio_init(pin, GPIO_IN);
    return 0;  // Assume success
}

// Function to read from DHT11
static int dht_read(gpio_t pin, int *temperature, int *humidity) {
    // Sample logic to read temperature and humidity from DHT11
    // Replace with actual interaction logic
    *temperature = 25; // Mock temperature
    *humidity = 60;    // Mock humidity
    return 0; // Assume success
}

int main(void) {
    gpio_t dht_pin = GPIO_PIN(0, 2);  // GPIO2
    int temp, hum;
    int init_res, read_res;

    printf("Starting DHT11 sensor reading...");

    // Initialize DHT11
    init_res = dht_init(dht_pin);
    if (init_res != 0) {
        printf("Failed to initialize DHT11 sensor\n");
        return 1;
    }

    while (1) {
        // Read DHT11
        read_res = dht_read(dht_pin, &temp, &hum);
        if (read_res == 0) {
            printf("Temperature: %d, Humidity: %d\n", temp, hum);
        } else {
            printf("Failed to read from DHT11 sensor\n");
        }

        // Wait for 5 seconds
        xtimer_sleep(5);
    }

    return 0;
}