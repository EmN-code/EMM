#include <xtimer.h>
#include <stdio.h>
#include <stdlib.h>
#include <periph/gpio.h>
#include "dht.h"

#define LED_PIN GPIO_PIN(PORT_B, 5) // Assuming LED is connected to this pin
#define DHT_PIN GPIO_PIN(PORT_A, 0) // Assuming DHT11 is connected to this pin
#define TEMP_THRESHOLD 30 // Temperature threshold in degrees Celsius
#define READ_INTERVAL (5U * US_PER_SEC) // 5 seconds in microseconds

int main(void) {
    // Initialize the GPIO pin for the LED
    gpio_init(LED_PIN, GPIO_OUT);
    gpio_clear(LED_PIN);

    // Initialize the DHT11 sensor
    dht_t dev;
    dht_params_t params;
    params.pin = DHT_PIN;
    params.type = DHT11;
    if (dht_init(&dev, &params) != 0) {
        printf("Error initializing DHT11\n");
        return 1;
    }

    while (1) {
        xtimer_sleep(5); // Wait for 5 seconds
        int16_t temp, hum;
        if (dht_read(&dev, &temp, &hum) != DHT_OK) {
            printf("Error reading DHT11\n");
            continue;
        }

        printf("Temperature: %d°C, Humidity: %d%%\n", temp, hum);

        if (temp > TEMP_THRESHOLD) {
            gpio_set(LED_PIN); // Turn on the LED
        } else {
            gpio_clear(LED_PIN); // Turn off the LED
        }
    }

    return 0;
}