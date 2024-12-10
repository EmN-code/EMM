#include "xtimer.h"
#include "periph/gpio.h"
#include "dht.h"

#define INTERVAL 5
#define TEMP_THRESHOLD 30
#define DHT_PIN GPIO_PIN(PORT_A, 0)
#define LED_PIN GPIO_PIN(PORT_A, 1)

dht_t dht_dev;

dht_params_t dht_params = {
    .pin = DHT_PIN,
    .type = DHT11
};

void check_temperature(void) {
    int16_t temp, hum;
    if (dht_read(&dht_dev, &temp, &hum) == DHT_OK) {
        printf("Temperature: %d°C, Humidity: %d%%\n", temp, hum);
        if (temp > TEMP_THRESHOLD) {
            gpio_set(LED_PIN);
            printf("Warning: Temperature exceeded threshold!\n");
        } else {
            gpio_clear(LED_PIN);
        }
    } else {
        printf("Failed to read DHT sensor\n");
    }
}

int main(void) {
    gpio_init(LED_PIN, GPIO_OUT);
    if (dht_init(&dht_dev, &dht_params) != DHT_OK) {
        printf("Failed to initialize DHT sensor\n");
        return 1;
    }

    while (1) {
        check_temperature();
        xtimer_sleep(INTERVAL);
    }

    return 0;
}