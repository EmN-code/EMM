#include "thread_arch.h"
#include "thread_config.h"
#include "thread_flags.h"
#include "timex.h"
#include "xtimer.h"
#include "periph/gpio.h"
#include "dht.h"
#include <stdio.h>

#define DHT_TYPE DHT11
#define DHT_PIN GPIO_PIN(0, 2)
#define READ_INTERVAL (5U * US_PER_SEC)

static dht_t dev;

void read_dht(void) {
    int16_t temp, hum;
    if (dht_read(&dev, &temp, &hum) == 0) {
        printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temp / 10, temp % 10, hum / 10, hum % 10);
    } else {
        puts("Failed to read data from DHT sensor\n");
    }
}

int main(void) {
    dht_params_t params;
    params.pin = DHT_PIN;
    params.type = DHT_TYPE;

    if (dht_init(&dev, &params) != 0) {
        puts("Failed to initialize DHT sensor\n");
        return 1;
    }

    while (1) {
        read_dht();
        xtimer_sleep(READ_INTERVAL / US_PER_SEC);
    }

    return 0;
}