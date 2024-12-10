
#include <stdio.h>
#include <xtimer.h>
#include "periph/gpio.h"
#include "dht11.h"  // Include the correct header file for DHT11 sensor

#define DHT11_PIN GPIO_PIN(0, 15)  // Example GPIO pin for DHT11 sensor
#define RGB_LED_PIN GPIO_PIN(0, 16)  // Example GPIO pin for RGB LED

#define RGB_LED_RED    (1 << 0)
#define RGB_LED_GREEN  (1 << 1)
#define RGB_LED_BLUE   (1 << 2)

#define RGB_LED_CYAN    (RGB_LED_GREEN | RGB_LED_BLUE)
#define RGB_LED_YELLOW  (RGB_LED_RED | RGB_LED_GREEN)
#define RGB_LED_MAGENTA (RGB_LED_RED | RGB_LED_BLUE)
#define RGB_LED_WHITE   (RGB_LED_RED | RGB_LED_GREEN | RGB_LED_BLUE)

void read_dht11(void) {
    int16_t temp, hum;
    if (dht11_read(DHT11_PIN, &temp, &hum) == DHT11_OK) {
        printf("Temperature: %d.%d C, Humidity: %d.%d %%\n", temp / 10, temp % 10, hum / 10, hum % 10);
    } else {
        puts("Failed to read from DHT11 sensor");
    }
}

void toggle_rgb_led(void) {
    static uint8_t state = 0;
    switch (state) {
        case 0:
            gpio_set(RGB_LED_PIN);
            state = 1;
            break;
        case 1:
            gpio_clear(RGB_LED_PIN);
            state = 0;
            break;
    }
}

int main(void) {
    // Initialize the DHT11 sensor
    if (dht11_init(DHT11_PIN) != DHT11_OK) {
        puts("Failed to initialize DHT11 sensor");
        return 1;
    }

    // Initialize the RGB LED
    gpio_init(RGB_LED_PIN, GPIO_OUT);

    while (1) {
        read_dht11();
        toggle_rgb_led();
        xtimer_sleep(5);  // Sleep for 5 seconds
    }

    return 0;
}
