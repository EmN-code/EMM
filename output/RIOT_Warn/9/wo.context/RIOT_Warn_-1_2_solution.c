#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "xtimer.h"
#include "thread.h"
#include "dht.h"
#include "periph/gpio.h"

#define DHT_PIN          GPIO_PIN(PORT_A, 0)
#define DHT_TYPE         DHT11
#define TEMP_THRESHOLD   30    // This is the temperature threshold for the LED to turn on
#define LED_PIN          GPIO_PIN(PORT_A, 1)

static dht_t dev;

int main(void) {
    // Initialize the DHT sensor
    if (dht_init(&dev, DHT_TYPE, DHT_PIN) != DHT_OK) {
        printf("Failed to initialize DHT sensor\n");
        return 1;
    }

    // Initialize the GPIO pin for the LED
    if (gpio_init(LED_PIN, GPIO_OUT) < 0) {
        printf("Failed to initialize LED_PIN\n");
        return 1;
    }

    int16_t temp, hum;
    while (1) {
        // Read data from the DHT sensor
        if (dht_read(&dev, &temp, &hum) != DHT_OK) {
            printf("Failed to read from DHT sensor\n");
        } else {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temp / 10, temp % 10, hum / 10, hum % 10);

            // Check if the temperature exceeds the threshold and turn on or off the LED accordingly
            if ((temp / 10) > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);
            } else {
                gpio_clear(LED_PIN);
            }
        }

        // Sleep for 5 seconds
        xtimer_sleep(5);
    }

    return 0;
}