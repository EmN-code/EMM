#include <stdio.h>
#include "xtimer.h"
#include "periph/gpio.h"
#include "dht.h"

#define DHT11_PIN GPIO_PIN(PORT_B, 6) // Pin where the DHT11 is connected
#define LED_PIN GPIO_PIN(PORT_A, 5)  // Pin where the LED is connected
#define READ_INTERVAL (5U * 1000000U) // 5 seconds in microseconds
#define TEMP_THRESHOLD 30 // Temperature threshold for the LED warning (in °C)

int main(void) {
    // Initialize the GPIO for the LED pin as output
    gpio_init(LED_PIN, GPIO_OUT);
    // Turn off the LED initially
    gpio_clear(LED_PIN);
    // Initialize sensor
    dht_t dev;
    dht_params_t params;
    params.pin = DHT11_PIN;
    dht_init(&dev, &params);

    int16_t temp, hum;

    while (1) {
        // Read temperature and humidity
        if (dht_read(&dev, &temp, &hum) != DHT_OK) {
            printf("Failed to read from sensor\n");
        } else {
            printf("Temp: %d.%d°C, Hum: %d.%d%%\n", temp / 10, temp % 10, hum / 10, hum % 10);

            // Check the temperature and turn on the LED if exceeds threshold
            if (temp / 10 > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);
            } else {
                gpio_clear(LED_PIN);
            }
        }

        // Sleep for the defined read interval
        xtimer_usleep(READ_INTERVAL);
    }

    return 0;
}