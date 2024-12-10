#include "timex.h"
#include "dht.h"
#include "xtimer.h"
#include "periph/gpio.h"

#define DHT_PIN GPIO_PIN(0, 0) // Adjust based on your wiring
#define LED_PIN GPIO_PIN(0, 1) // Adjust based on your wiring
#define TEMP_THRESHOLD 30      // Temperature threshold for the warning (°C)

int main(void) {
    dht_t dev;
    int16_t temp, hum;

    // Initialize the DHT sensor
    if (dht_init(&dev, &dht_config) != DHT_OK) {
        puts("Failed to initialize DHT sensor!");
        return 1;
    }

    // Initialize the LED
    if (gpio_init(LED_PIN, GPIO_OUT) != 0) {
        puts("Failed to initialize LED!");
        return 1;
    }

    while (1) {
        // Read temperature and humidity from DHT11
        if (dht_read(&dev, &temp, &hum) == DHT_OK) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temp / 10, temp % 10, hum / 10, hum % 10);
            
            // Check if the temperature exceeds the threshold
            if (temp / 10 > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);  // Turn on the LED
            } else {
                gpio_clear(LED_PIN); // Turn off the LED
            }
        } else {
            puts("Failed to read from DHT sensor!");
        }

        // Wait for 5 seconds
        xtimer_sleep(5);
    }

    return 0;
}