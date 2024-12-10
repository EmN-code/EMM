
#include <stdio.h>
#include <xtimer.h>
#include "periph/gpio.h"
#include "dht11.h"

#define DHT11_PIN GPIO_PIN(PORT_A, 1)
#define RGB_LED_PIN GPIO_PIN(PORT_A, 2)

void rgb_led_set(uint8_t colour) {
    gpio_write(RGB_LED_PIN, colour);
}

int main(void) {
    dht11_t dev;
    int status;
    uint16_t temp, hum;

    if (gpio_init(RGB_LED_PIN, GPIO_OUT) < 0) {
        puts("Failed to initialize RGB LED GPIO");
        return 1;
    }

    if (dht11_init(&dev, DHT11_PIN) != DHT11_OK) {
        puts("Failed to initialize DHT11 sensor");
        return 1;
    }

    while (1) {
        status = dht11_read(&dev, &hum, &temp);
        if (status == DHT11_OK) {
            printf("Temperature: %d C, Humidity: %d %%\n", temp, hum);
            rgb_led_set(RGB_LED_WHITE); // Turn on the RGB LED
        } else {
            puts("Failed to read from DHT11 sensor");
            rgb_led_set(RGB_LED_RED); // Turn on the RGB LED in red to indicate error
        }

        xtimer_sleep(5); // Wait for 5 seconds
    }

    return 0;
}
