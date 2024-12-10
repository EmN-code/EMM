#include <stdio.h>  // Standard I/O
#include "xtimer.h" // Timer functions
#include "periph/gpio.h"  // GPIO peripheral
#include "dht.h"  // DHT sensor functions
#include "thread.h" // Thread functions

// Defines
#define TEMP_THRESHOLD 30  // Temperature threshold in °C
#define SLEEP_INTERVAL (5U * US_PER_SEC)  // 5 second intervals
#define GPIO_PIN_LED GPIO_PIN(PORT_B, 6)  // GPIO pin for LED
#define GPIO_PIN_DHT GPIO_PIN(PORT_A, 0)  // GPIO pin for DHT sensor

// Main function
int main(void) {
    // Initialize DHT sensor
    dht_t dev;
    if (dht_init(&dev, GPIO_PIN_DHT, DHT11) != DHT_OK) {
        printf("[Error] Failed to initialize DHT11\n");
        return -1;
    }

    // Initialize LED pin
    if (gpio_init(GPIO_PIN_LED, GPIO_OUT) < 0) {
        printf("[Error] Failed to initialize LED pin\n");
        return -1;
    }

    printf("DHT11 Sensor & LED Controller Initialized\n");

    int16_t temp, hum;
    while (1) {
        // Read temperature and humidity
        if (dht_read(&dev, &temp, &hum) == DHT_OK) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temp / 10, temp % 10, hum / 10, hum % 10);

            // Check if temperature exceeds threshold
            if (temp / 10 > TEMP_THRESHOLD) {
                gpio_set(GPIO_PIN_LED);
                printf("Warning: Temperature exceeds threshold! LED turned on.\n");
            } else {
                gpio_clear(GPIO_PIN_LED);
            }
        } else {
            printf("[Error] Could not read data from DHT11\n");
        }

        // Sleep for the interval
        xtimer_sleep(SLEEP_INTERVAL / US_PER_SEC);
    }

    return 0;
}