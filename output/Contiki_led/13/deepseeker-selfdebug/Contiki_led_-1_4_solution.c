
#include <stdio.h>
#include <xtimer.h>
#include <dht11.h>
#include <periph/gpio.h>

#define DHT11_PIN GPIO_PIN(0, 1)
#define RGB_LED_PIN GPIO_PIN(0, 2)

#define RGB_LED_RED   0x01
#define RGB_LED_GREEN 0x02
#define RGB_LED_BLUE  0x04

#define RGB_LED_WHITE   (RGB_LED_RED | RGB_LED_GREEN | RGB_LED_BLUE)
#define RGB_LED_YELLOW  (RGB_LED_RED | RGB_LED_GREEN)
#define RGB_LED_CYAN    (RGB_LED_GREEN | RGB_LED_BLUE)
#define RGB_LED_MAGENTA (RGB_LED_RED | RGB_LED_BLUE)

void read_dht11(void) {
    int16_t temp, hum;
    if (dht11_read(DHT11_PIN, &temp, &hum) == 0) {
        printf("Temperature: %d.%d C, Humidity: %d.%d %%\n", temp / 10, temp % 10, hum / 10, hum % 10);
    } else {
        printf("Failed to read from DHT11 sensor\n");
    }
}

void set_rgb_led(uint8_t color) {
    gpio_write(RGB_LED_PIN, color);
}

int main(void) {
    xtimer_sleep(1); // Wait for the sensor to stabilize

    if (gpio_init(RGB_LED_PIN, GPIO_OUT) < 0) {
        printf("Failed to initialize RGB LED GPIO\n");
        return 1;
    }

    while (1) {
        read_dht11();
        set_rgb_led(RGB_LED_WHITE);
        xtimer_sleep(5);
    }

    return 0;
}
