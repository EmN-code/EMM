
#include "xtimer.h"
#include "periph/gpio.h"
#include "dht.h"

#define DHT_PIN             GPIO_PIN(0, 2)
#define DHT_PARAM_PULL      GPIO_IN_PU
#define SLEEP_SECS          5

static dht_t dev;

static void read_dht(void)
{
    int16_t temp, hum;
    if (dht_read(&dev, &temp, &hum) == DHT_OK) {
        printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
               temp / 10, temp % 10, hum / 10, hum % 10);
    } else {
        puts("Error reading DHT sensor");
    }
}

int main(void)
{
    dht_params_t params = {
        .pin = DHT_PIN,
        .in_mode = DHT_PARAM_PULL
    };

    if (dht_init(&dev, &params) == DHT_OK) {
        puts("DHT11 sensor initialized");
    } else {
        puts("Failed to initialize DHT11 sensor");
        return 1;
    }

    while (1) {
        read_dht();
        xtimer_sleep(SLEEP_SECS);
    }

    return 0;
}
