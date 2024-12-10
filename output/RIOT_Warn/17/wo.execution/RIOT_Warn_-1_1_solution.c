#include <stdio.h>
#include "xtimer.h"
#include "thread.h"
#include "periph/gpio.h"
#include "dht.h"

#define LED_PIN GPIO_PIN(0, 2)  // Example LED pin
#define TEMP_THRESHOLD 30        // Temperature threshold in degrees Celsius
#define SLEEP_INTERVAL (5U * US_PER_SEC)  // 5 seconds interval

dht_t dht11;

int main(void) {
    // Initialize GPIO for LED
    gpio_init(LED_PIN, GPIO_OUT);
    gpio_clear(LED_PIN);

    // Initialize DHT11 sensor
    dht_params_t dht_params;
    dht_params.pin = GPIO_PIN(0, 1); // Example data pin for DHT11
    dht_params.type = DHT11;

    if (dht_init(&dht11, &dht_params) != 0) {
        printf("Failed to initialize DHT11\n");
        return 1;
    }

    // Main loop
    while (1) {
        int16_t temp, hum;
        if (dht_read(&dht11, &temp, &hum) == 0) {
            // Convert temperature to degrees Celsius
            float temperature = temp / 10.0;
            printf("Temperature: %.1f°C, Humidity: %.1f%%\n", temperature, hum / 10.0);

            // Check if temperature exceeds threshold
            if (temperature >= TEMP_THRESHOLD) {
                gpio_set(LED_PIN);  // Turn on LED
                printf("Warning: High temperature!\n");
            } else {
                gpio_clear(LED_PIN);  // Turn off LED
            }
        } else {
            printf("Failed to read DHT11 sensor\n");
        }

        // Sleep for the defined interval
        xtimer_usleep(SLEEP_INTERVAL);
    }

    return 0;
}