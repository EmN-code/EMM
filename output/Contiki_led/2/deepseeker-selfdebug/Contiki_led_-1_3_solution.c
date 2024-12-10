
#include <stdio.h>
#include <xtimer.h>
#include <periph/gpio.h>
#include "dht11.h"

#define DHT11_PIN GPIO_PIN(0, 15)  // Replace with the actual GPIO pin connected to DHT11
#define RGB_LED_PIN GPIO_PIN(0, 16) // Replace with the actual GPIO pin connected to RGB LED

#define RGB_LED_RED   0x01
#define RGB_LED_GREEN 0x02
#define RGB_LED_BLUE  0x04

#define RGB_LED_WHITE  (RGB_LED_RED | RGB_LED_GREEN | RGB_LED_BLUE)

void turn_on_rgb_led(uint8_t color) {
    gpio_write(RGB_LED_PIN, color);
}

int main(void) {
    dht11_t dev;
    int16_t temp, hum;

    // Initialize the DHT11 sensor
    if (dht11_init(&dev, DHT11_PIN) == DHT11_OK) {
        printf("DHT11 sensor initialized successfully\n");
    } else {
        printf("Failed to initialize DHT11 sensor\n");
        return 1;
    }

    // Initialize the RGB LED
    gpio_init(RGB_LED_PIN, GPIO_OUT);

    while (1) {
        // Read temperature and humidity from DHT11
        if (dht11_read(&dev, &temp, &hum) == DHT11_OK) {
            printf("Temperature: %d C, Humidity: %d %%\n", temp, hum);
            // Turn on the RGB LED
            turn_on_rgb_led(RGB_LED_WHITE);
        } else {
            printf("Failed to read data from DHT11 sensor\n");
        }

        // Wait for 5 seconds
        xtimer_sleep(5);
    }

    return 0;
}
