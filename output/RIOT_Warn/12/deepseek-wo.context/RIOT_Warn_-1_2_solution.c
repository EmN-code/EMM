
#include "xtimer.h"
#include "periph/gpio.h"
#include "dht.h"
#include <stdio.h>

#define DHT_PIN             GPIO_PIN(0, 18)
#define LED_PIN             GPIO_PIN(0, 21)
#define TEMP_THRESHOLD      25
#define INTERVAL_MS         5000000

static dht_t dht_dev;

void read_dht11(void) {
    int16_t temp, hum;
    if (dht_read(&dht_dev, &temp, &hum) == DHT_OK) {
        printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
               temp / 10, temp % 10, hum / 10, hum % 10);
        if (temp / 10 > TEMP_THRESHOLD) {
            gpio_set(LED_PIN);
        } else {
            gpio_clear(LED_PIN);
        }
    } else {
        puts("Error reading DHT11 sensor");
    }
}

int main(void) {
    xtimer_ticks32_t last_wakeup = xtimer_now();

    if (gpio_init(LED_PIN, GPIO_OUT) < 0) {
        puts("Error initializing LED GPIO");
        return 1;
    }

    dht_params_t dht_params = {
        .pin = DHT_PIN,
        .type = DHT11,
        .in_mode = GPIO_IN_PU
    };

    if (dht_init(&dht_dev, &dht_params) == DHT_OK) {
        puts("DHT11 sensor initialized");
    } else {
        puts("Error initializing DHT11 sensor");
        return 1;
    }

    while (1) {
        read_dht11();
        xtimer_periodic_wakeup(&last_wakeup, INTERVAL_MS);
    }

    return 0;
}
