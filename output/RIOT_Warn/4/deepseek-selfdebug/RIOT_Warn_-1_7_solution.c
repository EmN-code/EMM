
#include "xtimer.h"
#include "periph/gpio.h"
#include "dht.h"

#define DHT_PIN             GPIO_PIN(0, 18)
#define LED_PIN             GPIO_PIN(0, 21)
#define TEMP_THRESHOLD      30
#define INTERVAL_MS         5000000

static dht_t dht_dev;

int main(void) {
    int16_t temp, hum;

    // Initialize the DHT11 sensor
    if (dht_init(&dht_dev, DHT_PIN) == DHT_OK) {
        printf("DHT11 sensor initialized successfully\n");
    } else {
        printf("Failed to initialize DHT11 sensor\n");
        return 1;
    }

    // Initialize the LED pin as output
    gpio_init(LED_PIN, GPIO_OUT);
    gpio_set(LED_PIN);

    while (1) {
        // Read temperature and humidity from the DHT11 sensor
        if (dht_read(&dht_dev, &temp, &hum) == DHT_OK) {
            printf("Temperature: %d C, Humidity: %d %%\n", temp, hum);

            // Check if the temperature exceeds the threshold
            if (temp > TEMP_THRESHOLD) {
                gpio_clear(LED_PIN);  // Turn on the LED
            } else {
                gpio_set(LED_PIN);    // Turn off the LED
            }
        } else {
            printf("Failed to read data from DHT11 sensor\n");
        }

        // Wait for 5 seconds before the next reading
        xtimer_usleep(INTERVAL_MS);
    }

    return 0;
}
