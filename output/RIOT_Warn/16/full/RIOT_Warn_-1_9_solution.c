#include <xtimer.h>
#include <stdio.h>
#include <stdlib.h>
#include "dht.h"  // This header should match your RIOT setup

// Define the GPIO pins used for the DHT11 sensor and LED
#define DHT11_PIN     GPIO_PIN(PORT_A, 5)
#define LED_PIN       GPIO_PIN(PORT_B, 0)

// Define the temperature threshold
#define TEMP_THRESHOLD 30

// Define the interval at which the DHT11 sensor is read, in microseconds
#define INTERVAL      (5U * 1000000U)

int main(void) {
    // Initialize the xtimer
    xtimer_init();

    // Initialize the DHT sensor and LED pin
    if (dht_init(DHT11_PIN) != 0) {
        printf("Error: Unable to initialize DHT11 sensor.\n");
        return 1;
    }

    gpio_init(LED_PIN, GPIO_OUT);

    // variable to hold temperature and humidity
    int16_t temperature, humidity;

    while (1) {
        // Read the values from the DHT11 sensor
        int result = dht_read(DHT11_PIN, &temperature, &humidity);
        if (result == 0) {
            printf("Temperature: %d.%d C, Humidity: %d.%d %%\n", temperature / 10, temperature % 10, humidity / 10, humidity % 10);
            if (temperature / 10 > TEMP_THRESHOLD) {
                // Turn on the LED
                gpio_set(LED_PIN);
            } else {
                // Turn off the LED
                gpio_clear(LED_PIN);
            }
        } else {
            printf("Error: Could not read data from DHT11 sensor.\n");
        }

        // Sleep for the defined interval
        xtimer_usleep(INTERVAL);
    }

    return 0;
}