#include <xtimer.h>
#include <stdio.h>
#include <periph/gpio.h>
#include <dht.h>

#define DHT_PIN GPIO_PIN(0, 5) // Example GPIO pin
#define LED_PIN GPIO_PIN(0, 2) // Example GPIO pin for LED
#define THRESHOLD_TEMPERATURE 30 // Temperature threshold for LED warning
#define SLEEP_INTERVAL (5U * US_PER_SEC) // 5 seconds

int main(void) {
    // Initialize DHT sensor
    if (dht_init(&dev, &params) != DHT_OK) {
        printf("Error: Unable to initialize DHT sensor\n");
        return 1;
    }

    // Configure LED pin as output
    gpio_init(LED_PIN, GPIO_OUT);

    int16_t temperature, humidity;
    while (1) {
        // Read DHT sensor
        if (dht_read(&dev, &temperature, &humidity) != DHT_OK) {
            printf("Error: Unable to read from DHT sensor\n");
        } else {
            // Convert temperature to Celsius if needed and check threshold
            int temp_celsius = temperature / 10;
            printf("Temperature: %d C, Humidity: %d%%\n", temp_celsius, humidity);

            // Turn LED on or off based on threshold
            if (temp_celsius > THRESHOLD_TEMPERATURE) {
                gpio_set(LED_PIN);
            } else {
                gpio_clear(LED_PIN);
            }
        }

        // Wait 5 seconds
        xtimer_sleep(SLEEP_INTERVAL);
    }

    return 0;
}