#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <xtimer.h>
#include <periph/gpio.h>
#include <dht.h>

#define DHT_PIN GPIO_PIN(PORT_X, Y) // ensure you replace X and Y with the actual port and pin numbers
#define LED_PIN GPIO_PIN(PORT_X, Y) // replace X and Y with the actual port and pin numbers
#define TEMP_THRESHOLD 30 // temperature threshold in degree Celsius

static dht_t dht_dev;

int main(void) {
    // Initialize DHT sensor
    dht_params_t dht_params;
    dht_params.pin = DHT_PIN;
    dht_params.type = DHT11;
    dht_init(&dht_dev, &dht_params);

    // Initialize LED pin
    gpio_init(LED_PIN, GPIO_OUT);
    gpio_clear(LED_PIN);

    while (1) {
        int16_t temp, hum;

        // Read values from sensor
        if (dht_read(&dht_dev, &temp, &hum) != DHT_OK) {
            printf("Error reading from DHT sensor\n");
        } else {
            printf("Temperature: %d, Humidity: %d\n", temp, hum);

            // Check if temperature exceeds threshold
            if (temp > TEMP_THRESHOLD * 10) { // DHT sensor returns value multiplied by 10
                gpio_set(LED_PIN);
            } else {
                gpio_clear(LED_PIN);
            }
        }

        // Sleep for 5 seconds
        xtimer_sleep(5);
    }

    return 0;
}