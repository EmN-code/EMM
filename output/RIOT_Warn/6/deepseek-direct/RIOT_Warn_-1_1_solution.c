
#include <stdio.h>
#include "xtimer.h"
#include "periph/gpio.h"
#include "dht.h"

#define DHT_PIN             GPIO_PIN(0, 18)  // GPIO pin for DHT11 sensor
#define LED_PIN             GPIO_PIN(0, 2)   // GPIO pin for LED
#define TEMP_THRESHOLD      25               // Temperature threshold in Celsius
#define INTERVAL            5                // Interval in seconds

static void read_dht11(int16_t *temp, int16_t *hum) {
    dht_t dev;
    dht_init(&dev, DHT_TYPE_DHT11, DHT_PIN);
    dht_read(&dev, temp, hum);
}

int main(void) {
    int16_t temperature, humidity;

    // Initialize LED pin as output
    gpio_init(LED_PIN, GPIO_OUT);
    gpio_set(LED_PIN);  // Turn off LED initially

    while (1) {
        // Read DHT11 sensor
        read_dht11(&temperature, &humidity);

        // Print temperature and humidity
        printf("Temperature: %d C, Humidity: %d %%\n", temperature, humidity);

        // Check if temperature exceeds the threshold
        if (temperature > TEMP_THRESHOLD) {
            gpio_clear(LED_PIN);  // Turn on LED
        } else {
            gpio_set(LED_PIN);    // Turn off LED
        }

        // Wait for 5 seconds
        xtimer_sleep(INTERVAL);
    }

    return 0;
}
