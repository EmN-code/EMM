#include "xtimer.h"
#include "periph/gpio.h"
#include "dht.h"

#define LED_PIN GPIO_PIN(0, 1)
#define DHT_PIN GPIO_PIN(0, 2)
#define TEMP_THRESHOLD 25

int main(void) {
    gpio_init(LED_PIN, GPIO_OUT);
    gpio_clear(LED_PIN);

    dht_t dev;
    dht_params_t params = {
        .type = DHT_TYPE_DHT11,
        .pin = DHT_PIN
    };

    if (dht_init(&dev, &params) != DHT_OK) {
        puts("DHT initialization failed\n");
        return 1;
    }

    while (1) {
        int16_t temp, hum;
        if (dht_measure(&dev, &temp, &hum) != DHT_OK) {
            puts("Failed to read data from DHT sensor\n");
        } else {
            printf("Temperature: %d°C
", temp / 10);
            printf("Humidity: %d%%
", hum / 10);
            if (temp / 10 > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);
            } else {
                gpio_clear(LED_PIN);
            }
        }
        xtimer_sleep(5);
    }
    return 0;
}