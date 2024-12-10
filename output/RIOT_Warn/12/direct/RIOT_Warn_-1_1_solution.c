#include <stdio.h>
#include "ztimer.h"
#include "periph/gpio.h"
#include "dht.h"

#define DHT_PIN GPIO_PIN(0, 4) // Define the GPIO pin for the DHT11
#define LED_PIN GPIO_PIN(0, 2) // Define the GPIO pin for the LED
#define TEMP_THRESHOLD 30 // Temperature threshold in Celsius
#define INTERVAL (5U * US_PER_SEC) // 5 seconds interval

int main(void) {
    dht_t dev;
    int16_t temperature, humidity;

    // Initialize the DHT sensor
    if (dht_init(&dev, DHT_PIN, DHT11) != DHT_OK) {
        puts("Failed to initialize DHT11 sensor");
        return 1;
    }

    // Initialize the LED pin
    if (gpio_init(LED_PIN, GPIO_OUT)) {
        puts("Failed to initialize LED pin");
        return 1;
    }

    while (1) {
        // Read the temperature and humidity
        if (dht_read(&dev, &temperature, &humidity) == DHT_OK) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temperature / 10, temperature % 10, humidity / 10, humidity % 10);

            // Check temperature threshold
            if (temperature / 10 > TEMP_THRESHOLD) {
                // Turn on LED
                gpio_set(LED_PIN);
            } else {
                // Turn off LED
                gpio_clear(LED_PIN);
            }
        } else {
            puts("Failed to read from DHT11 sensor");
        }

        // Sleep for the defined interval
        ztimer_sleep(ZTIMER_MSEC, INTERVAL / 1000);
    }

    return 0;
}