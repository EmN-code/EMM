#include "dht.h"
#include "xtimer.h"
#include "periph/gpio.h"

#define DHT_PIN GPIO_PIN(0, 1)  // Assume the DHT is connected to GPIO1
#define LED_PIN GPIO_PIN(0, 2)  // Assume the LED is connected to GPIO2
#define TEMP_THRESHOLD 25       // Set temperature threshold to 25 degrees Celsius
#define READ_INTERVAL (5U * US_PER_SEC)  // Read every 5 seconds

dht_t dev;
dht_params_t params = {.pin = DHT_PIN, .type = DHT11};

int main(void) {
    // Initialize the DHT sensor
    if (dht_init(&dev, &params) != DHT_OK) {
        printf("Failed to initialize DHT sensor\n");
        return 1;
    }

    // Initialize the LED pin
    gpio_init(LED_PIN, GPIO_OUT);
    gpio_clear(LED_PIN);

    int16_t temp, hum;
    while (1) {
        // Read the sensor data
        if (dht_read(&dev, &temp, &hum) != DHT_OK) {
            printf("Failed to read from DHT sensor\n");
        } else {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temp / 10, temp % 10, hum / 10, hum % 10);

            // Check if the temperature exceeds the threshold
            if (temp / 10 > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);  // Turn on LED
            } else {
                gpio_clear(LED_PIN);  // Turn off LED
            }
        }

        // Sleep for the defined interval
        xtimer_sleep(READ_INTERVAL / US_PER_SEC);
    }

    return 0;
}