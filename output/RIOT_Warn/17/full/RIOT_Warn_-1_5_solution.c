#include <xtimer.h>
#include <periph/gpio.h>
#include "dht.h"

#define DHT_PIN GPIO_PIN(0, 4)
#define LED_PIN GPIO_PIN(0, 16)
#define TEMP_THRESHOLD 30

static dht_t dev;

int main(void) {
    int16_t temperature, humidity;
    int rc;

    gpio_init(LED_PIN, GPIO_OUT);
    gpio_clear(LED_PIN);

    while (1) {
        rc = dht_init(&dev, DHT_TYPE_DHT11, DHT_PIN);

        if (rc != DHT_OK) {
            printf("DHT initialization failed\n");
            xtimer_sleep(1);
            continue;
        }

        rc = dht_read(&dev, &humidity, &temperature);

        if (rc == DHT_OK) {
            printf("Temperature: %d°C, Humidity: %d%%\n", temperature, humidity);

            if (temperature > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);
            } else {
                gpio_clear(LED_PIN);
            }
        } else {
            printf("Failed to read data from DHT sensor\n");
        }

        xtimer_sleep(5);
    }

    return 0;
}