#include "periph/gpio.h"
#include "xtimer.h"
#include <stdio.h>

#define DHT_PIN GPIO_PIN(PORT_A, 10)
#define LED_PIN GPIO_PIN(PORT_B, 5)
#define TEMP_THRESHOLD 30

static void read_dht11(void) {
    int16_t temp, hum;
    dht_t dev;
    dht_params_t params = {
        .pin = DHT_PIN,
        .type = DHT11
    };

    if (dht_init(&dev, &params) != DHT_OK) {
        printf("Failed to initialize DHT11\n");
        return;
    }

    if (dht_read(&dev, &temp, &hum) != DHT_OK) {
        printf("Failed to read data from DHT11\n");
        return;
    }

    printf("Temperature: %d, Humidity: %d\n", temp, hum);

    if (temp > TEMP_THRESHOLD) {
        gpio_set(LED_PIN);
        printf("Warning: Temperature exceeded threshold\n");
    } else {
        gpio_clear(LED_PIN);
    }
}

int main(void) {
    gpio_init(LED_PIN, GPIO_OUT);
    while (1) {
        read_dht11();
        xtimer_sleep(5);
    }
    return 0;
}