
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "xtimer.h"
#include "periph/gpio.h"
#include "dht.h"

#define DHT11_PIN GPIO_PIN(0, 4)   // Defined GPIO pin number for DHT11
#define LED_PIN GPIO_PIN(0, 5)     // Defined GPIO pin number for LED
#define TEMP_THRESHOLD 30          // Temperature threshold for triggering LED

int main(void) {
    gpio_init(LED_PIN, GPIO_OUT);
    dht_t dev;
    if (dht_init(&dev, DHT11, DHT11_PIN) != DHT_OK) {
        printf("DHT initialization failed!\n");
        return 1;
    }

    int16_t temp, hum;

    while (1) {
        // Retrieve DHT11 data
        if (dht_read(&dev, &temp, &hum) != DHT_OK) {
            printf("Failed to read from DHT sensor\n");
        } else {
            // Convert temperature to Celsius
            int temp_celsius = temp / 10;
            printf("Temperature: %d C, Humidity: %d %%\n", temp_celsius, hum / 10);

            // Check temperature threshold
            if (temp_celsius > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);
                printf("Warning: High temperature! LED turned on\n");
            } else {
                gpio_clear(LED_PIN);
                printf("Temperature normal. LED turned off\n");
            }
        }
        xtimer_sleep(5);
    }

    return 0;
}