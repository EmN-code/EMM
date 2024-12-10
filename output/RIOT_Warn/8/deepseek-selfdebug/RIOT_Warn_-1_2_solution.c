
#include "xtimer.h"
#include "periph/gpio.h"
#include "dht.h"

#define DHT_PIN             GPIO_PIN(0, 18)
#define LED_PIN             GPIO_PIN(0, 21)
#define TEMP_THRESHOLD      30
#define INTERVAL_MS         5000000

static dht_t dht_dev;

void init_dht(void) {
    dht_params_t dht_params = {
        .pin = DHT_PIN,
        .type = DHT11,
        .in_mode = GPIO_IN_PU
    };
    dht_init(&dht_dev, &dht_params);
}

void init_led(void) {
    gpio_init(LED_PIN, GPIO_OUT);
    gpio_set(LED_PIN);
}

int main(void) {
    int16_t temp, hum;

    init_dht();
    init_led();

    while (1) {
        if (dht_read(&dht_dev, &temp, &hum) == DHT_OK) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
                   temp / 10, temp % 10, hum / 10, hum % 10);

            if (temp > TEMP_THRESHOLD * 10) {
                gpio_clear(LED_PIN);
            } else {
                gpio_set(LED_PIN);
            }
        } else {
            puts("Failed to read from DHT sensor");
        }

        xtimer_usleep(INTERVAL_MS);
    }

    return 0;
}
