#include <xtimer.h>
#include <stdio.h>
#include <periph/gpio.h>
#include <dht.h>

#define DHT_PIN         GPIO_PIN(0, 5)
#define LED_PIN         GPIO_PIN(0, 6)
#define TEMP_THRESHOLD  30  // Temperature threshold in Celsius
#define SLEEP_PERIOD    (5 * US_PER_SEC)  // 5 seconds

int main(void) {
    dht_t dev;
    int16_t temp, hum;
    int result;

    // Initialize LED pin
    if (gpio_init(LED_PIN, GPIO_OUT)) {
        puts("Error: Could not initialize LED pin");
        return 1;
    }

    // Continuous setup retry loop for DHT11 sensor
    while (1) {
        result = dht_init(&dev, DHT_PIN, DHT11);
        if (result == DHT_OK) {
            printf("DHT11 initialized successfully\n");
            break;
        } else {
            printf("Failed to initialize DHT11. Retrying...\n");
            xtimer_sleep(1);
        }
    }

    while (1) {
        // Read temperature and humidity values
        if (dht_read(&dev, &temp, &hum) != DHT_OK) {
            printf("Error: Could not read data from DHT11\n");
        } else {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
                    temp / 10, temp % 10, hum / 10, hum % 10);

            // Check if temperature exceeds threshold
            if (temp / 10 >= TEMP_THRESHOLD) {
                gpio_set(LED_PIN);
                puts("Warning: Temperature threshold exceeded! LED turned on.");
            } else {
                gpio_clear(LED_PIN);
            }
        }

        // Sleep for 5 seconds
        xtimer_usleep(SLEEP_PERIOD);
    }

    return 0;
}