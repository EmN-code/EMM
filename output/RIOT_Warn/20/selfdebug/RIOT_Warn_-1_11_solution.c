#include <stdio.h>
#include <stdlib.h>
#include "xtimer.h"
#include "periph/gpio.h"
#include "dht.h"

#define DHT_PIN GPIO_PIN(PORT_A, 0)  // Assuming GPIO pin defined for DHT
#define LED_PIN GPIO_PIN(PORT_A, 1)  // Assuming GPIO pin defined for LED
#define TEMP_THRESHOLD 25            // Temperature threshold in Celsius
#define SLEEP_INTERVAL (5U * 1000000U) // Sleep for 5 seconds

int main(void) {
    dht_t dev;
    int16_t temp, hum;

    // Initialize the DHT device
    if (dht_init(&dev, DHT11, DHT_PIN) != DHT_OK) {
        printf("Failed to initialize DHT sensor\n");
        return 1;
    }

    // Initialize the LED pin
    if (gpio_init(LED_PIN, GPIO_OUT)) {
        printf("Failed to initialize LED pin\n");
        return 1;
    }

    while (1) {
        if (dht_read(&dev, &temp, &hum) == DHT_OK) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
                temp / 10, temp % 10, hum / 10, hum % 10);
            if (temp / 10 >= TEMP_THRESHOLD) {
                gpio_set(LED_PIN);
                printf("LED turned on due to high temperature!\n");
            } else {
                gpio_clear(LED_PIN);
                printf("Temperature within normal range.\n");
            }
        } else {
            printf("Failed to read from DHT sensor\n");
        }

        // Sleep for the defined interval
        xtimer_usleep(SLEEP_INTERVAL);
    }

    return 0;
}