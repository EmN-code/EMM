#include <periph/rtc.h>
#include <periph/i2c.h>
#include <periph/gpio.h>
#include <mma8x5x_params.h>
#include <dht_params.h>
#include <dht.h>

#define LED_PIN GPIO_PIN(0, 2)
#define DHT11_PIN GPIO_PIN(0, 4)
#define TEMPERATURE_THRESHOLD 30

void init_gpio(void) {
    gpio_init(LED_PIN, GPIO_OUT);
    gpio_clear(LED_PIN);
}

int main(void) {
    printf("Starting application...\n");
    init_gpio();

    dht_t dev;
    dev.pin = DHT11_PIN;
    dev.type = DHT_TYPE_DHT11;

    if (dht_init(&dev) != 0) {
        printf("DHT11 initialization failed\n");
        return 1;
    }

    while (1) {
        int16_t temperature, humidity;
        if (dht_read(&dev, &temperature, &humidity) != 0) {
            printf("Failed to read from DHT11\n");
        } else {
            printf("Temperature: %d, Humidity: %d\n", temperature, humidity);
            if (temperature > TEMPERATURE_THRESHOLD) {
                gpio_set(LED_PIN);
            } else {
                gpio_clear(LED_PIN);
            }
        }

        xtimer_sleep(5);
    }

    return 0;
}