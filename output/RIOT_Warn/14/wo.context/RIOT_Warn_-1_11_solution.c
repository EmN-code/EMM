#include "xtimer.h"
#include "periph/gpio.h"
#include "dht.h"

#define DHT_PIN         GPIO_PIN(PORT_A, 4)  // Assuming PORT_A and pin 4
#define LED_PIN         GPIO_PIN(PORT_B, 5)  // Assuming PORT_B and pin 5
#define TEMP_THRESHOLD  30  // Temperature threshold in celsius
#define DELAY           5U   // Delay in seconds

static dht_t dev;

int main(void) {
    // Initialize DHT sensor
    dht_params_t dht_params;
    dht_params.pin = DHT_PIN;
    dht_params.type = DHT11;
    if (dht_init(&dev, &dht_params) != DHT_OK) {
        printf("DHT init failed\n");
        return 1;
    }

    // Initialize LED pin
    if (gpio_init(LED_PIN, GPIO_OUT)) {
        printf("Error to initialize LED\n");
        return 1;
    }

    int16_t temperature, humidity;
    while (1) {
        // Read from DHT11
        if (dht_read(&dev, &temperature, &humidity) == DHT_OK) {
            printf("Temperature: %d, Humidity: %d
", temperature / 10, humidity / 10);
            // Check if temperature exceeds threshold
            if ((temperature / 10) > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);
            } else {
                gpio_clear(LED_PIN);
            }
        }
        else {
            printf("Failed to read from DHT11\n");
        }
        // Wait for 5 seconds
        xtimer_sleep(DELAY);
    }
    return 0;
}