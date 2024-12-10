
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
        .in_mode = GPIO_IN_PULLUP  // Corrected the parameter name
    };
    dht_init(&dht_dev, &dht_params);
}

void read_dht(void) {
    int16_t temp, hum;
    if (dht_read(&dht_dev, &temp, &hum) == DHT_OK) {
        printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temp / 10, temp % 10, hum / 10, hum % 10);
        if (temp / 10 > TEMP_THRESHOLD) {
            gpio_set(LED_PIN);
        } else {
            gpio_clear(LED_PIN);
        }
    } else {
        puts("Error reading DHT sensor");
    }
}

int main(void) {
    gpio_init(LED_PIN, GPIO_OUT);
    init_dht();

    while (1) {
        read_dht();
        xtimer_usleep(INTERVAL_MS);
    }

    return 0;
}
