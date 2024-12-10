
#include <stdio.h>
#include "xtimer.h"  // Include the missing header file
#include "periph/gpio.h"
#include "dht11.h"
#include "board.h"

#define DHT11_PIN GPIO_PIN(DHT11_CONFIGURE_GPIO_PORT, DHT11_CONFIGURE_GPIO_PIN)
#define RGB_LED_PIN GPIO_PIN(0, 5)  // Assuming the RGB LED is connected to GPIO pin 5

void read_dht11(dht11_t *dev, int16_t *temp, int16_t *hum) {
    dht11_read(dev, temp, hum);
}

void set_rgb_led(gpio_t pin, uint8_t color) {
    gpio_write(pin, color);
}

int main(void) {
    dht11_t dht11;
    int16_t temperature, humidity;

    // Initialize DHT11 sensor
    dht11_init(&dht11, DHT11_PIN);

    // Initialize RGB LED
    gpio_init(RGB_LED_PIN, GPIO_OUT);

    while (1) {
        // Read DHT11 sensor
        read_dht11(&dht11, &temperature, &humidity);

        // Turn on RGB LED based on temperature (example: red for high temp, blue for low temp)
        if (temperature > 25) {
            set_rgb_led(RGB_LED_PIN, RGB_LED_RED);
        } else {
            set_rgb_led(RGB_LED_PIN, RGB_LED_BLUE);
        }

        // Wait for 5 seconds
        xtimer_sleep(5);
    }

    return 0;
}
