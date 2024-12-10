#include "xtimer.h"
#include "dht.h"
#include "dht_params.h"
#include "periph/gpio.h"

#define DHT_PIN         GPIO_PIN(0, 2)
#define READ_INTERVAL   (5U)

dht_t dev;

int main(void) {
    xtimer_ticks32_t last = xtimer_now();

    dht_params_t params = {
        .pin = DHT_PIN,
        .type = DHT11
    };

    if (dht_init(&dev, &params) != DHT_OK) {
        printf("DHT initialization failed\n");
        return 1;
    }

    while (1) {
        int16_t temp, hum;
        if (dht_read(&dev, &temp, &hum) == DHT_OK) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
                   temp / 10, temp % 10, hum / 10, hum % 10);
        } else {
            printf("Failed to read DHT sensor\n");
        }

        xtimer_periodic_wakeup(&last, READ_INTERVAL * US_PER_SEC);
    }

    return 0;
}