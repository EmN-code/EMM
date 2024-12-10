#include <stdio.h>
#include <stdint.h>
#include "xtimer.h"
#include "periph/gpio.h"
#include "dht.h"

#define DHT_PIN     GPIO_PIN(PORT_B, 2)  // GPIO pin connected to the DHT sensor
#define LED_PIN     GPIO_PIN(PORT_B, 4)  // GPIO pin connected to the LED
#define THRESHOLD   25  // Temperature threshold in Celsius

int main(void) {
    dht_t dev;
    int16_t temperature, humidity;
    int rc;

    // Initialize the DHT sensor
    if (dht_init(&dev, DHT_TYPE_DHT11, DHT_PIN) != DHT_OK) {
        printf("Failed to initialize DHT sensor\n");
        return 1;
    }

    printf("DHT sensor initialized\n");

    // Configure LED pin as output
    gpio_init(LED_PIN, GPIO_OUT);

    while (1) {
        // Read temperature and humidity from the DHT sensor
        rc = dht_read(&dev, &temperature, &humidity);
        if (rc != DHT_OK) {
            printf("Failed to read DHT sensor\n");
            xtimer_sleep(1);
            continue;
        }

        printf("Temperature: %d°C, Humidity: %d%%\n", temperature, humidity);

        // Check if temperature exceeds the threshold
        if (temperature > THRESHOLD) {
            gpio_set(LED_PIN);
            printf("Warning: Temperature exceeded threshold\n");
        } else {
            gpio_clear(LED_PIN);
        }

        // Wait for 5 seconds before the next read
        xtimer_sleep(5);
    }

    return 0;
}