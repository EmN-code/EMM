#include "xtimer.h"
#include "periph/gpio.h"
#include "dht.h"

#define DHT11_PIN GPIO_PIN(PORT_A, 10)  // Define your DHT11 pin
#define LED_PIN GPIO_PIN(PORT_A, 15)    // Define your LED pin
#define TEMP_THRESHOLD 30               // Temperature threshold in degree Celsius
#define INTERVAL (5U * US_PER_SEC)      // 5 seconds interval

static dht_t dht11_dev;

int main(void) {
    // Initialize DHT sensor
    if (dht_init(&dht11_dev, DHT11_PIN, DHT_TYPE_DHT11) != DHT_OK) {
        puts("Failed to initialize DHT11 sensor");
        return 1;
    }

    // Initialize LED pin
    if (gpio_init(LED_PIN, GPIO_OUT)) {
        puts("Failed to initialize LED pin");
        return 1;
    }

    // Turn off LED initially
    gpio_clear(LED_PIN);

    int16_t temp, hum;
    while (1) {
        // Read temperature from DHT11
        if (dht_read(&dht11_dev, &temp, &hum) == DHT_OK) {
            // Check if temperature exceeds threshold
            if (temp / 10 > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);  // Turn on LED
            } else {
                gpio_clear(LED_PIN);  // Turn off LED
            }
        } else {
            puts("Failed to read from DHT11 sensor");
        }

        // Wait for the next interval
        xtimer_sleep(INTERVAL);
    }

    return 0;
}