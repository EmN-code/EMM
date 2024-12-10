#include "xtimer.h"
#include "periph/gpio.h"
#include "dht.h"

#define DHT_PIN GPIO_PIN(PORT_A, 0)
#define LED_PIN GPIO_PIN(PORT_B, 0)
#define TEMP_THRESHOLD 30
#define SLEEP_TIME (5 * US_PER_SEC)

int main(void) {
    dht_t dev;

    // Initialize DHT sensor with specified pin and type
    if (dht_init(&dev, DHT_PIN, DHT11) != 0) {
        printf("Failed to initialize DHT sensor\n");
        return 1;
    }

    // Initialize LED pin as output
    gpio_init(LED_PIN, GPIO_OUT);

    int16_t temp, hum;

    while (1) {
        // Read values from DHT sensor
        if (dht_read(&dev, &temp, &hum) != 0) {
            printf("Failed to read data from DHT sensor\n");
        } else {
            // Convert temperature from tenths of a degree to degrees
            printf("Temperature: %d°C, Humidity: %d%%\n", temp / 10, hum / 10);

            // Turn LED on if the temperature exceeds the threshold
            if (temp / 10 > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);
            } else {
                gpio_clear(LED_PIN);
            }
        }

        // Sleep for specified time
        xtimer_sleep(SLEEP_TIME);
    }

    return 0;
}