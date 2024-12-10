
#include <stdio.h>
#include "xtimer.h"
#include "periph/gpio.h"
#include "dht11.h"

#define RGB_LED_RED    (1 << 0)
#define RGB_LED_GREEN  (1 << 1)
#define RGB_LED_BLUE   (1 << 2)

#define RGB_LED_WHITE   (RGB_LED_RED | RGB_LED_GREEN | RGB_LED_BLUE)
#define RGB_LED_MAGENTA (RGB_LED_RED | RGB_LED_BLUE)
#define RGB_LED_YELLOW  (RGB_LED_RED | RGB_LED_GREEN)
#define RGB_LED_CYAN    (RGB_LED_GREEN | RGB_LED_BLUE)

#define DHT11_GPIO_PORT   (0)
#define DHT11_GPIO_PIN    (1)

#define DHT11_VALUE_HUMIDITY_INTEGER       (0)
#define DHT11_VALUE_TEMPERATURE_INTEGER    (2)
#define DHT11_VALUE_TEMPERATURE_DECIMAL    (3)

#define DHT11_STATUS_TIMEOUT            (1)
#define DHT11_STATUS_OKAY               (0)

#define RGB_LED_GPIO_PORT (0)
#define RGB_LED_GPIO_PIN  (2)

void read_dht11(dht11_t *dev, int16_t *temp, int16_t *hum) {
    int status = dht11_read(dev, temp, hum);
    if (status == DHT11_STATUS_OKAY) {
        printf("Temperature: %d.%d C, Humidity: %d%%\n", *temp / 10, *temp % 10, *hum);
    } else {
        printf("Failed to read DHT11 sensor\n");
    }
}

void toggle_rgb_led(gpio_t led_pin) {
    static int state = 0;
    state = !state;
    gpio_write(led_pin, state);
}

int main(void) {
    dht11_t dev;
    int16_t temperature, humidity;

    gpio_init(DHT11_GPIO_PORT, DHT11_GPIO_PIN, GPIO_OUT);
    dht11_init(&dev, DHT11_GPIO_PORT, DHT11_GPIO_PIN);

    gpio_init(RGB_LED_GPIO_PORT, RGB_LED_GPIO_PIN, GPIO_OUT);

    while (1) {
        read_dht11(&dev, &temperature, &humidity);
        toggle_rgb_led(RGB_LED_GPIO_PIN);
        xtimer_sleep(5);
    }

    return 0;
}
