#include <stdio.h>  // For printf
#include "xtimer.h"  // For timing functions, such as xtimer_sleep
#include "thread.h"  // For threading, but primarily here it's included for any timing related threading mechanism
#include "periph/gpio.h"  // For GPIO operations (i.e., controlling the LED)
#include "dht.h"  // For interfacing with the DHT sensor

#define DHT_PIN GPIO_PIN(0, 4)  // Pin where DHT sensor is connected
#define LED_PIN GPIO_PIN(0, 2)  // Pin where your LED is connected
#define TEMP_THRESHOLD 30  // Temperature threshold for warning
#define SLEEP_INTERVAL 5   // Time interval between reads in seconds

int main(void) {
    // Initialize DHT sensor
dht_t dev;
const dht_params_t params = {
        .pin = DHT_PIN,
        .type = DHT11
    };
    
    if (dht_init(&dev, &params) != DHT_OK) {
        printf("Failed to initialize DHT sensor\n");
        return 1;
    }

    // Initialize LED pin
gpio_init(LED_PIN, GPIO_OUT);

    while (1) {
        int16_t temp, hum;

        // Read values from DHT11
        if (dht_read(&dev, &temp, &hum) == DHT_OK) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temp / 10, temp % 10, hum / 10, hum % 10);

            // Check if the temperature exceeds the threshold
            if ((temp / 10) > TEMP_THRESHOLD) {
                // Turn on the LED
                gpio_set(LED_PIN);
                printf("Warning: Temperature exceeds the threshold! LED is ON.\n");
            } else {
                // Turn off the LED
                gpio_clear(LED_PIN);
                printf("Temperature is normal. LED is OFF.\n");
            }
        } else {
            printf("Failed to read from DHT sensor\n");
        }

        // Sleep for the defined interval
        xtimer_sleep(SLEEP_INTERVAL);
    }

    return 0;
}