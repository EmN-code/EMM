// Include necessary headers
#include "xtimer.h"
#include "periph/gpio.h"
#include "dht.h"

// Define DHT sensor and LED pin
#define DHT_PIN GPIO_PIN(PORT_B, 5)
#define LED_PIN GPIO_PIN(PORT_B, 0)

// Temperature threshold for LED warning
#define TEMP_THRESHOLD 30

int main(void) {
    dht_t dev;
    int16_t temp, hum;

    // Initialize DHT sensor
    if (dht_init(&dev, DHT_PIN, DHT11) != DHT_OK) {
        // Handle initialization error here
        return 1;
    }

    gpio_init(LED_PIN, GPIO_OUT);

    while (1) {
        // Read temperature and humidity
        if (dht_read(&dev, &temp, &hum) != DHT_OK) {
            // Handle read error here
            continue;
        }

        // Check if temperature exceeds threshold
        if (temp > TEMP_THRESHOLD) {
            gpio_set(LED_PIN);
        } else {
            gpio_clear(LED_PIN);
        }

        // Wait for 5 seconds
        xtimer_sleep(5);
    }

    return 0;
}