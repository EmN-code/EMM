
#include <stdio.h>
#include <xtimer.h>
#include <periph/gpio.h>
#include "dht11.h"

#define DHT11_PIN GPIO_PIN(0, 15)  // Replace with the actual GPIO pin connected to DHT11
#define RGB_LED_PIN GPIO_PIN(0, 16) // Replace with the actual GPIO pin connected to RGB LED

int main(void) {
    dht11_t dev;
    int16_t temp, hum;

    // Initialize DHT11 sensor
    if (dht11_init(&dev, DHT11_PIN) == DHT11_OK) {
        printf("DHT11 sensor initialized successfully\n");
    } else {
        printf("Failed to initialize DHT11 sensor\n");
        return 1;
    }

    // Initialize RGB LED
    gpio_init(RGB_LED_PIN, GPIO_OUT);

    while (1) {
        // Read temperature and humidity from DHT11
        if (dht11_read(&dev, &temp, &hum) == DHT11_OK) {
            printf("Temperature: %d C, Humidity: %d %%\n", temp, hum);

            // Turn on RGB LED
            gpio_set(RGB_LED_PIN);
        } else {
            printf("Failed to read data from DHT11 sensor\n");
        }

        // Wait for 5 seconds
        xtimer_sleep(5);

        // Turn off RGB LED
        gpio_clear(RGB_LED_PIN);
    }

    return 0;
}
