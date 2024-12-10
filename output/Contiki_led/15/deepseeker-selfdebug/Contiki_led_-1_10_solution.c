
#include "xtimer.h"
#include "dht11.h"
#include "periph/gpio.h"

#define DHT11_PIN GPIO_PIN(DHT11_CONFIGURE_GPIO_PORT, 0)
#define RGB_LED_PIN GPIO_PIN(0, 1) // Assuming GPIO port 0, pin 1 for RGB LED

void rgb_led_set(uint8_t colour) {
    gpio_write(RGB_LED_PIN, colour);
}

int main(void) {
    dht11_t dev;
    int16_t temp, hum;

    // Initialize DHT11 sensor
    dht11_init(&dev, DHT11_PIN);

    // Initialize RGB LED
    gpio_init(RGB_LED_PIN, GPIO_OUT);

    while (1) {
        // Read DHT11 sensor
        if (dht11_read(&dev, &temp, &hum) == DHT11_OK) {
            // Turn on RGB LED based on temperature
            if (temp < 20) {
                rgb_led_set(RGB_LED_BLUE);
            } else if (temp >= 20 && temp < 30) {
                rgb_led_set(RGB_LED_GREEN);
            } else {
                rgb_led_set(RGB_LED_RED);
            }
        }

        // Wait for 5 seconds
        xtimer_sleep(5);
    }

    return 0;
}
