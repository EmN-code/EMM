#include "ztimer.h"
#include "xtimer.h"
#include "stdio.h"
#include "stdbool.h"
#include "stdlib.h"
#include "periph/gpio.h"

// Define GPIO pins for ESP32
#define LED_PIN GPIO_NUM_2
#define DHT11_PIN GPIO_NUM_4

// Threshold temperature in degree Celsius
#define TEMP_THRESHOLD 30

void initialize_gpio(void) {
    // Initialize LED pin
    gpio_init(LED_PIN, GPIO_OUT);
    // Set LED off initially
    gpio_clear(LED_PIN);
}

int main(void) {
    initialize_gpio();
    dht_t dev;
    dht_params_t params;

    // Initialize DHT11 sensor
    params.pin = DHT11_PIN;
    dht_init(&dev, &params);

    int16_t temperature, humidity;

    while (1) {
        // Read DHT11 sensor data
        if (dht_read(&dev, &temperature, &humidity) != ESP_OK) {
            printf("Failed to read from DHT11 sensor\n");
        } else {
            printf("Temperature: %d.%d C, Humidity: %d.%d\n",
                   temperature / 10, temperature % 10,
                   humidity / 10, humidity % 10);

            // Check if the temperature exceeds the threshold
            if (temperature / 10 > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);  // Turn on the LED
            } else {
                gpio_clear(LED_PIN);  // Turn off the LED
            }
        }

        // Wait for 5 seconds
        xtimer_sleep(5);
    }

    return 0;
}