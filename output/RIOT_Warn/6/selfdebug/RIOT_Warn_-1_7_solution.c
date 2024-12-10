#include <stdio.h>
#include "xtimer.h"
#include "periph/gpio.h"
#include "dht.h"

#define DHT_PIN GPIO_PIN(PORT_A, 0)
#define DHT_TYPE DHT11
#define LED_PIN GPIO_PIN(PORT_B, 1)
#define TEMP_THRESHOLD 30  // temperature threshold for LED warning

int main(void)
{
    dht_t dev;
    int16_t temp, hum;

    // Initialize the DHT sensor
    if (dht_init(&dev, DHT_PIN, DHT_TYPE) != 0) {
        printf("Failed to initialize DHT sensor\n");
        return 1;
    }

    // Set LED pin as output
    gpio_init(LED_PIN, GPIO_OUT);

    while (1) {
        // Read temperature and humidity
        if (dht_read(&dev, &temp, &hum) != 0) {
            printf("Failed to read DHT sensor\n");
        } else {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temp / 10, temp % 10, hum / 10, hum % 10);

            // Check if temperature exceeds threshold
            if (temp / 10 > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);  // Turn on LED
            } else {
                gpio_clear(LED_PIN);  // Turn off LED
            }
        }

        // Wait 5 seconds before next read
        xtimer_sleep(5);
    }

    return 0;
}