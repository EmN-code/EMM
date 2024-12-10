#include <xtimer.h>
#include <stdio.h>
#include <stdbool.h>
#include "periph/gpio.h"
#include "dht.h"

#define DHT11_PIN GPIO_PIN(PORT_A, 0) // Ensure that the right GPIO pin is defined
#define LED_PIN GPIO_PIN(PORT_A, 1)
#define TEMP_THRESHOLD 30
#define SLEEP_INTERVAL 5

int main(void) {
    dht_t dev;
    int16_t temperature, humidity;

    // Initialize the DHT11 sensor
    if (dht_init(&dev, DHT11_PIN, DHT11) != 0) {
        puts("DHT11 sensor initialization failed");
        return 1;
    }

    // Initialize the LED pin
    if (gpio_init(LED_PIN, GPIO_OUT) != 0) {
        puts("Failed to initialize LED pin");
        return 1;
    }

    while (true) {
        // Attempt to read from DHT11
        if (dht_read(&dev, &temperature, &humidity) != 0) {
            puts("Failed to read from DHT11");
        } else {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
                   temperature / 10, temperature % 10, humidity / 10, humidity % 10);

            // Check temperature threshold
            if (temperature > TEMP_THRESHOLD * 10) {  // Convert to proper scale
                gpio_set(LED_PIN);
                puts("Temperature threshold exceeded! LED turned on.");
            } else {
                gpio_clear(LED_PIN);
            }
        }
        // Wait for the next interval
        xtimer_sleep(SLEEP_INTERVAL);
    }

    return 0;
}