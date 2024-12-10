#include <xtimer.h>
#include <stdio.h>
#include "periph/gpio.h"
#include "dht.h"

#define DHT_PIN  GPIO_PIN(PORT_A, 5)  // Example pin for DHT11 data
#define LED_PIN  GPIO_PIN(PORT_A, 6)  // Example pin for LED
#define TEMP_THRESHOLD 30 // Temperature threshold for warning, adjust accordingly

void check_temperature(void) {
    dht_t dev;
    int16_t temp, hum; // Temperature and humidity variables
    dht_params_t params = {
        .pin = DHT_PIN,
        .type = DHT11
    };

    // Initialize DHT sensor
    if (dht_init(&dev, &params) != DHT_OK) {
        printf("DHT initialization failed\n");
        return;
    }

    while (1) {
        // Read DHT sensor
        if (dht_read(&dev, &temp, &hum) == DHT_OK) {
            temp /= 10; // Convert to degrees Celsius
            printf("Temperature: %d°C\n", temp);
        }
        else {
            printf("Failed to read from DHT sensor\n");
        }

        // Check temperature against threshold
        if (temp > TEMP_THRESHOLD) {
            gpio_set(LED_PIN);
            printf("Warning: high temperature!\n");
        } else {
            gpio_clear(LED_PIN);
        }

        xtimer_sleep(5); // Wait for 5 seconds
    }
}

int main(void) {
    // Initialize LED GPIO
    if (gpio_init(LED_PIN, GPIO_OUT)) {
        printf("Failed to initialize LED GPIO\n");
        return 1;
    }

    printf("Starting DHT monitor\n");
    check_temperature(); // Start temperature monitoring
    return 0;
}