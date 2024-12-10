
#include <stdio.h>
#include <xtimer.h>
#include "dht11.h"
#include "periph/gpio.h"

#define DHT11_GPIO_PIN GPIO_PIN(PORT_A, 0)
#define RGB_LED_PIN GPIO_PIN(PORT_A, 1)

void rgb_led_set(uint8_t colour) {
    gpio_write(RGB_LED_PIN, colour);
}

int main(void) {
    dht11_t dev;
    int16_t temp, hum;

    // Initialize the DHT11 sensor
    if (dht11_init(&dev, DHT11_GPIO_PIN) != DHT11_OK) {
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
            rgb_led_set(1);
        } else {
            printf("Failed to read from DHT11 sensor\n");
            // Turn off the RGB LED
            rgb_led_set(0);
        }

        // Wait for 5 seconds
        xtimer_sleep(5);
    }

    return 0;
}
