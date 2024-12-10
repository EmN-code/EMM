#include <xtimer.h>
#include <periph/gpio.h>
#include <stdio.h>
#include "dht.h" // Ensure you have dht.h included in your project

#define DHT_PIN GPIO_PIN(0, 1)   // Change to the proper pin you're using for DHT11
#define LED_PIN GPIO_PIN(0, 2)   // Change to the pin connected to your LED
#define TEMP_THRESHOLD 30       // Temperature threshold for LED warning

static dht_params_t dht_params;

int main(void) {
    // Initialize the LED pin
    if (gpio_init(LED_PIN, GPIO_OUT)) {
        printf("Error: unable to initialize LED pin\n");
        return 1;
    }

    // Initialize the DHT11 sensor
    dht_params.pin = DHT_PIN;
    dht_params.type = DHT11;

    if (dht_init(&dht_params) != DHT_OK) {
        printf("Error: unable to initialize DHT sensor\n");
        return 1;
    }

    // Main loop
    while (1) {
        int16_t temp, humidity;
        // Read values
        if (dht_read(&dht_params, &temp, &humidity) == DHT_OK) {
            float temperature = (float)temp / 10.0;
            printf("Temperature: %.1f°C, Humidity: %d%%\n", temperature, humidity);

            // Turn on LED if temperature exceeds threshold
            if (temperature > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);
            } else {
                gpio_clear(LED_PIN);
            }
        } else {
            printf("Error reading data from DHT sensor\n");
        }

        // Wait for 5 seconds between readings
        xtimer_sleep(5);
    }
}