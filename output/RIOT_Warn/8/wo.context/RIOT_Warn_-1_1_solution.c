#include <xtimer.h>            // for using xtimer_sleep
#include <stdio.h>             // for printf
#include "dht.h"             // for DHT sensor functions
#include "periph/gpio.h"    // for GPIO functions and LED control

#define DHT_PIN GPIO_PIN(PORT_A, 0)
#define LED_PIN GPIO_PIN(PORT_A, 1)
#define TEMP_THRESHOLD 30  // Temperature threshold in degrees Celsius
#define READ_INTERVAL (5U) // Interval in seconds

int main(void)
{
    // Initialize DHT11 sensor
    dht_t dev;
    dht_params_t dht_params;
    dht_params.pin = DHT_PIN;
    dht_params.type = DHT11;
    
    // Check initialization
    if (dht_init(&dev, &dht_params) != DHT_OK) {
        printf("DHT initialization failed\n");
        return 1;
    }

    // Initialize LED pin as output
    gpio_init(LED_PIN, GPIO_OUT);
    printf("Setup complete. Starting sensor read loop...\n");

    while (1) {
        int16_t temp, hum;
        // Read temperature and humidity
        if (dht_read(&dev, &temp, &hum) != DHT_OK) {
            printf("Failed to read from DHT sensor\n");
        } else {
            // Convert temperature from deci-degrees to degrees
            float temperature = temp / 10.0;
            float humidity = hum / 10.0;
            printf("Temperature: %.1f°C, Humidity: %.1f%%\n", temperature, humidity);
            // Check temperature threshold
            if (temperature > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);
                printf("Temperature exceeds threshold! LED ON\n");
            } else {
                gpio_clear(LED_PIN);
                printf("Temperature normal. LED OFF\n");
            }
        }
        // Sleep for the specified interval
        xtimer_sleep(READ_INTERVAL);
    }
    return 0;
}