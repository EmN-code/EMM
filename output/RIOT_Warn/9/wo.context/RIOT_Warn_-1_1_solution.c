#include <xtimer.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "periph/gpio.h"
#include "dht.h"

#define DHT_PIN GPIO_PIN(PORT_A, 0)     // Define the GPIO pin connected to DHT11
#define LED_PIN GPIO_PIN(PORT_A, 1)     // Define the GPIO pin connected to the LED
#define TEMP_THRESHOLD 30               // Define the temperature threshold
#define DELAY_BETWEEN_READINGS (5U * US_PER_SEC)  // Delay of 5 seconds

int main(void) {
    // Initialize the DHT11 sensor
    dht_t dev;
    dht_params_t params;
    params.pin = DHT_PIN;
    params.type = DHT_TYPE_DHT11;

    if (dht_init(&dev, &params) != DHT_OK) {
        puts("DHT11 initialization failed");
        return 1;
    }

    // Initialize the LED pin as output
    gpio_init(LED_PIN, GPIO_OUT);

    int16_t temp, hum;
    while (1) {
        // Read the temperature and humidity from the DHT11
        if (dht_read(&dev, &temp, &hum) != DHT_OK) {
            puts("Failed to read from DHT11 sensor");
        } else {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", 
                   temp / 10, temp % 10, hum / 10, hum % 10);

            // Turn on LED if temperature exceeds threshold
            if ((temp / 10) > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);
            } else {
                gpio_clear(LED_PIN);
            }
        }

        // Wait for 5 seconds before next reading
        xtimer_usleep(DELAY_BETWEEN_READINGS);
    }

    return 0;
}