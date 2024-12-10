// Necessary RIOT and platform-specific includes
#include <stdint.h>
#include <stdio.h>
#include "xtimer.h"
#include "periph/gpio.h"
#include "dht.h"

// Configuration
#define DHT_PIN GPIO_PIN(PORT_A, 5)  // Adjust GPIO port as needed
#define LED_PIN GPIO_PIN(PORT_A, 3)  // Adjust GPIO port as needed
#define TEMP_THRESHOLD 30             // Temperature threshold in Celsius
#define SLEEP_INTERVAL (5U * US_PER_SEC) // 5 seconds interval

int main(void) {
    // DHT Sensor configuration
    dht_params_t dht_params;
    dht_params.pin = DHT_PIN;
    dht_params.type = DHT11;

    dht_t dev;

    // Initialize DHT sensor
    if (dht_init(&dev, &dht_params) != DHT_OK) {
        printf("DHT sensor initialization failed\n");
        return 1;
    }

    // Initialize LED pin as output
    gpio_init(LED_PIN, GPIO_OUT);

    // Main loop
    while (1) {
        uint16_t temperature, humidity;

        // Read temperature and humidity data from DHT11
        if (dht_read(&dev, &temperature, &humidity) != DHT_OK) {
            printf("Error reading data from DHT sensor\n");
        }
        else {
            // Convert temperature value to Celsius
            float temp_c = ((float)temperature) / 10;
            printf("Temperature: %.1f°C, Humidity: %d%%\n", temp_c, humidity);

            // Control LED based on temperature threshold
            if (temp_c > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);
                printf("Temperature exceeds threshold! LED is ON.\n");
            }
            else {
                gpio_clear(LED_PIN);
                printf("Temperature is normal. LED is OFF.\n");
            }
        }

        // Sleep for defined interval
        xtimer_sleep(SLEEP_INTERVAL / US_PER_SEC);
    }

    return 0;
}