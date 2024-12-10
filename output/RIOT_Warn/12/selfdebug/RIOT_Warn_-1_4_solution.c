#include "xtimer.h"
#include "periph/gpio.h"

#define DHT_PIN GPIO_PIN(X, Y) // Replace X and Y with real port and pin numbers for the DHT11
#define LED_PIN GPIO_PIN(A, B) // Replace A and B with real port and pin numbers for the LED
#define THRESHOLD_TEMP 30 // Temperature threshold for LED warning

int main(void) {
    // Initialize DHT sensor
    dht_t dev;
    if (dht_init(&dev, DHT_PIN, DHT11) != DHT_OK) {
        printf("Failed to initialize DHT11\n");
        return 1;
    }

    // Initialize LED pin as output
    if (gpio_init(LED_PIN, GPIO_OUT)) {
        printf("Error to initialize LED_PIN\n");
        return 1;
    }

    int16_t temp, hum;

    while (1) {
        // Read temperature and humidity
        if (dht_read(&dev, &temp, &hum) != DHT_OK) {
            printf("Failed to read from DHT11\n");
        } else {
            printf("Temperature: %d, Humidity: %d\n", temp, hum);

            // Check if temperature exceeds threshold
            if (temp > THRESHOLD_TEMP) {
                gpio_set(LED_PIN);
            } else {
                gpio_clear(LED_PIN);
            }
        }

        // Wait 5 seconds before next reading
        xtimer_sleep(5);
    }

    return 0;
}