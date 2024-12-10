#include <xtimer.h>
#include <stdio.h>
#include "periph/gpio.h"
#include "dht.h"

#define DHT_PIN GPIO_PIN(0, 4)   // Define the pin where the DHT11 is connected
#define LED_PIN GPIO_PIN(0, 5)   // Define the pin where the LED is connected
#define THRESHOLD 30             // Temperature threshold for the LED warning

static dht_t dev;

int main(void) {

    // Initialize the DHT sensor
    if (dht_init(&dev, DHT_PIN, DHT11) != DHT_OK) {
        printf("DHT initialization failed\n");
        return 1;
    }

    // Initialize the LED
    gpio_init(LED_PIN, GPIO_OUT);

    int16_t temperature, humidity;
    while (1) {
        // Read values from DHT11 sensor
        if (dht_read(&dev, &temperature, &humidity) != DHT_OK) {
            printf("Failed to read from DHT sensor\n");
        } else {
            printf("Temperature: %d, Humidity: %d\n", temperature, humidity);

            // Compare temperature and control LED
            if (temperature > THRESHOLD) {
                printf("Temperature exceeded threshold! Turning on LED.\n");
                gpio_set(LED_PIN);
            } else {
                gpio_clear(LED_PIN);
            }
        }

        // Wait 5 seconds
        xtimer_sleep(5);
    }
    return 0;
}
