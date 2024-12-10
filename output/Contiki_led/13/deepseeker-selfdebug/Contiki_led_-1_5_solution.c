
#include <stdio.h>
#include <xtimer.h>
#include "periph/gpio.h"
#include "dht11.h"

#define DHT11_PIN GPIO_PIN(DHT11_CONFIGURE_GPIO_PORT, DHT11_CONFIGURE_GPIO_PIN)
#define RGB_LED_PIN GPIO_PIN(0, 1) // Assuming GPIO port 0 and pin 1 for RGB LED

void read_dht11(void) {
    int16_t temp, hum;
    if (dht11_read(DHT11_PIN, &temp, &hum) == 0) {
        printf("Temperature: %d, Humidity: %d\n", temp, hum);
    } else {
        printf("Failed to read DHT11 sensor\n");
    }
}

void rgb_led_control(void) {
    static uint8_t color = 0;
    color = (color + 1) % 8; // Cycle through 8 colors
    rgb_led_set(color);
}

int main(void) {
    // Initialize DHT11 sensor
    if (dht11_init(DHT11_PIN) < 0) {
        printf("Failed to initialize DHT11 sensor\n");
        return 1;
    }

    // Initialize RGB LED
    gpio_init(RGB_LED_PIN, GPIO_OUT);

    while (1) {
        read_dht11();
        rgb_led_control();
        xtimer_sleep(5); // Sleep for 5 seconds
    }

    return 0;
}
