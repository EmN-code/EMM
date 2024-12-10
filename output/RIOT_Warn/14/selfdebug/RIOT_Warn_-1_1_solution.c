#include <xtimer.h>  // Provides for timer functions
#include <stdio.h>  // Standard input/output functions
#include <stdlib.h>  // Standard library functions
#include "dht.h"  // DHT sensor interface
#include "periph/gpio.h"  // GPIO functions

#define DHT_PIN GPIO_PIN(0, 0)  // Define the GPIO pin for DHT11
#define LED_PIN GPIO_PIN(0, 1)  // Define the GPIO pin for LED
#define TEMP_THRESHOLD 30  // Temperature threshold
#define DHT_TYPE DHT11  // DHT11 type

static dht_t dev;

int main(void) {
    // Initialize the DHT sensor
    if (dht_init(&dev, &((dht_params_t)DHT_PARAMS(DHT_PIN, DHT_TYPE))) != 0) {
        printf("Failed to initialize DHT sensor\n");
        return 1;
    }
    // Initialize the LED pin
    if (gpio_init(LED_PIN, GPIO_OUT) != 0) {
        printf("Failed to initialize LED pin\n");
        return 1;
    }
    printf("DHT sensor initialized and LED pin configured\n");

    while (1) {
        int16_t temp, hum;
        // Read the DHT sensor
        if (dht_read(&dev, &temp, &hum) != DHT_OK) {
            printf("Failed to read from DHT sensor\n");
        } else {
            float temp_celsius = temp / 10.0;
            printf("Temperature: %.1f°C, Humidity: %.1f%%\n", temp_celsius, hum / 10.0);
            // Check temperature and turn on/off LED
            if (temp_celsius > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);
                printf("LED ON: Temperature exceeded threshold\n");
            } else {
                gpio_clear(LED_PIN);
                printf("LED OFF: Temperature below threshold\n");
            }
        }
        // Wait for 5 seconds
        xtimer_sleep(5);
    }
    return 0;
}