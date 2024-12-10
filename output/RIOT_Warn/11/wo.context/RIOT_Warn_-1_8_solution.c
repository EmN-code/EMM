#include <xtimer.h>
#include <stdio.h>
#include <periph/gpio.h>
#include "dht.h"

#define DHT_PARAM    DHT_PARAM(PIN)
#define DHT_TYPE     DHT11
#define TEMP_THRESHOLD 30  // Temperature threshold in Celsius
#define LED_PIN      GPIO_PIN(0, 2) // Assume GPIO_PIN(0, 2) for LED usage
#define SLEEP_SEC    (5U)

int main(void) {
    // Initialize the DHT11 sensor
    if (dht_init(&DHT_PARAM) != 0) {
        printf("Error: unable to initialize DHT sensor\n");
        return 1;
    }

    // Initialize the LED pin
    gpio_init(LED_PIN, GPIO_OUT);

    int16_t temp, hum;

    while (1) {
        // Read the DHT11 sensor
        if (dht_read(&DHT_PARAM, &temp, &hum) != DHT_OK) {
            printf("Error: unable to read data from DHT sensor\n");
        } else {
            printf("Temperature: %d°C, Humidity: %d%%\n", temp / 10, hum / 10);

            // Check if the temperature exceeds the threshold
            if ((temp / 10) > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);
            } else {
                gpio_clear(LED_PIN);
            }
        }

        // Sleep for 5 seconds
        xtimer_sleep(SLEEP_SEC);
    }

    return 0;
}