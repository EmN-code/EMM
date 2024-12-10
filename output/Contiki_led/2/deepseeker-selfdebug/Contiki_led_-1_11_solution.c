
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include "dht11_sensor.h"
#include "gpio.h"

#define DHT11_GPIO_PIN GPIO_PIN(0, 15)  // Example GPIO pin for DHT11
#define RGB_LED_PIN GPIO_PIN(0, 16)     // Example GPIO pin for RGB LED

void rgb_led_set(uint8_t colour) {
    gpio_write(RGB_LED_PIN, colour);
}

int main(void) {
    int16_t humidity, temperature;
    int status;

    // Initialize GPIO pins
    gpio_init(DHT11_GPIO_PIN, GPIO_OUT);
    gpio_init(RGB_LED_PIN, GPIO_OUT);

    while (1) {
        // Read DHT11 sensor
        status = dht11_read(&humidity, &temperature);

        if (status == DHT11_STATUS_OKAY) {
            printf("Humidity: %d.%d %%\n", humidity / 10, humidity % 10);
            printf("Temperature: %d.%d °C\n", temperature / 10, temperature % 10);

            // Turn on RGB LED
            rgb_led_set(RGB_LED_WHITE);
        } else {
            printf("Failed to read DHT11 sensor\n");
            rgb_led_set(RGB_LED_RED);  // Turn on red LED to indicate error
        }

        // Wait for 5 seconds
        sleep(5);
    }

    return 0;
}
