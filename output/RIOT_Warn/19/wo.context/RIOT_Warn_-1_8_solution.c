#include <stdio.h>
#include <xtimer.h>
#include <stdbool.h>
#include <periph/gpio.h>
#include <dht.h>

#define DHT_PIN GPIO_PIN(0, 4)  // Correct esp32's GPIO pin for DHT11
#define LED_PIN GPIO_PIN(0, 5)  // Choose another GPIO pin for the LED
#define TEMP_THRESHOLD 30  // Temperature threshold in Celsius

int main(void) {
    dht_t dev;

    // Initialize GPIO for LED
    if (gpio_init(LED_PIN, GPIO_OUT) < 0) {
        printf("Error to initialize GPIO\n");
        return 1;
    }

    // Initialize DHT11; correct argument count and types
    if (dht_init(&dev, DHT_PIN, DHT11) != DHT_OK) {
        printf("Error initializing DHT11\n");
        return 1;
    }

    while (1) {
        int16_t temp, hum;

        // Attempt to read from DHT11
        if (dht_read(&dev, &temp, &hum) != DHT_OK) {
            printf("Failed to read from sensor\n");
        } else {
            printf("Temperature: %d °C, Humidity: %d %%RH\n", temp, hum);
            // Check if temperature exceeds threshold
            if (temp > TEMP_THRESHOLD * 10) {  // Multiply by 10 to match DHT's scaling
                gpio_set(LED_PIN);
            } else {
                gpio_clear(LED_PIN);
            }
        }
        // Wait for 5 seconds (5 * 10^6 microseconds)
        xtimer_usleep(5000000);
    }
    return 0;
}