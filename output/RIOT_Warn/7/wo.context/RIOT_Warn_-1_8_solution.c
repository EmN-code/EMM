#include "xtimer.h"
#include "periph/gpio.h"
#include "dht.h"
#include <stdio.h>

#define DHT_PIN GPIO_PIN(PORT_A, 0) // Adjust these pins as needed
#define LED_PIN GPIO_PIN(PORT_A, 1) // Adjust these pins as needed
#define TEMP_THRESHOLD 25 // Temperature threshold in degrees Celsius

int main(void) {
    dht_t dev;
    int16_t temperature, humidity;

    // Initialize DHT sensor
    if (dht_init(&dev, DHT_PIN, DHT11) != DHT_OK) {
        printf("Failed to initialize DHT sensor\n");
        return 1;
    }

    // Initialize LED pin
    gpio_init(LED_PIN, GPIO_OUT);

    while (1) {
        if (dht_read(&dev, &temperature, &humidity) != DHT_OK) {
            printf("Failed to read from DHT sensor\n");
        } else {
            printf("Temperature: %d°C, Humidity: %d%%\n", temperature, humidity);

            if (temperature > TEMP_THRESHOLD) {
                // Turn on LED
                gpio_set(LED_PIN);
            } else {
                // Turn off LED
                gpio_clear(LED_PIN);
            }
        }

        // Wait 5 seconds
        xtimer_sleep(5);
    }

    return 0;
}