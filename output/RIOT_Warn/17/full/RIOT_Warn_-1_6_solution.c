#include <xtimer.h>  // Timer functions
#include <stdio.h>    // Standard Input/Output
#include <periph/gpio.h>  // GPIO related functions
#include <dht.h>      // DHT sensor functions

#define DHT_PIN         GPIO_PIN(0, 23)  // Set to your specific DHT pin
#define LED_PIN         GPIO_PIN(0, 25)  // Set to your specific LED pin
#define TEMP_THRESHOLD  30  // Temperature threshold for warning
#define INTERVAL        (5 * US_PER_SEC)  // 5-second interval

int main(void) {
    int16_t temperature, humidity;
    dht_t dev;
    int rc;

    // Initialize DHT sensor
    while (1) {
        rc = dht_init(&dev, DHT_PIN, DHT11);
        if (rc != 0) {
            printf("Failed to initialize DHT sensor. Retrying...\n");
            xtimer_sleep(1);
        } else {
            printf("DHT sensor initialized successfully.\n");
            break;
        }
    }

    // Configure LED pin
    gpio_init(LED_PIN, GPIO_OUT);
    gpio_clear(LED_PIN);

    // Main loop
    while (1) {
        // Read data from DHT11 sensor
        rc = dht_read(&dev, &temperature, &humidity);
        if (rc != 0) {
            printf("Failed to read from sensor, rc = %d. Retrying in 5 seconds...\n", rc);
            xtimer_usleep(INTERVAL);
            continue;
        }

        printf("Temperature: %d, Humidity: %d\n", temperature, humidity);

        // Check if temperature exceeds threshold
        if (temperature > TEMP_THRESHOLD) {
            printf("Warning: Temperature exceeded threshold!\n");
            gpio_set(LED_PIN);  // Turn on the LED
        } else {
            gpio_clear(LED_PIN);  // Turn off the LED
        }

        // Wait 5 seconds before next read
        xtimer_usleep(INTERVAL);
    }
    return 0;
}
