
#include <stdio.h>
#include <xtimer.h>
#include "periph/gpio.h"
#include "dht11.h"

#define DHT11_PIN GPIO_PIN(0, 15)
#define RGB_LED_PIN GPIO_PIN(0, 16)

#define RGB_LED_RED   0x01
#define RGB_LED_GREEN 0x02
#define RGB_LED_BLUE  0x04

#define RGB_LED_WHITE   (RGB_LED_RED | RGB_LED_GREEN | RGB_LED_BLUE)
#define RGB_LED_YELLOW  (RGB_LED_RED | RGB_LED_GREEN)
#define RGB_LED_MAGENTA (RGB_LED_RED | RGB_LED_BLUE)

void rgb_led_set(uint8_t colour) {
    gpio_write(RGB_LED_PIN, colour);
}

int main(void) {
    dht11_t dev;
    int16_t temp, hum;
    uint8_t status;

    gpio_init(RGB_LED_PIN, GPIO_OUT);
    dht11_init(&dev, DHT11_PIN);

    while (1) {
        status = dht11_read(&dev, &temp, &hum);
        if (status == DHT11_STATUS_OKAY) {
            printf("Temperature: %d.%d C, Humidity: %d.%d %%\n",
                   temp / 10, temp % 10, hum / 10, hum % 10);
            rgb_led_set(RGB_LED_WHITE);
        } else {
            printf("Failed to read DHT11 sensor\n");
            rgb_led_set(RGB_LED_MAGENTA);
        }

        xtimer_sleep(5);
    }

    return 0;
}
