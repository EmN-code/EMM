
#include "xtimer.h"
#include "periph/gpio.h"
#include "dht.h"

#define DHT_PIN GPIO_PIN(0, 2)
#define DHT_TYPE DHT11
#define SLEEP_INTERVAL (5 * 1000 * 1000U) /* 5 seconds */

int main(void)
{
    dht_params_t dht_params = {
        .pin = DHT_PIN,
        .type = DHT_TYPE,
        .in_mode = DHT_PARAM_PULL, /* Ensure this is defined */
    };

    dht_t dev;
    int16_t temp, hum;

    if (dht_init(&dev, &dht_params) != DHT_OK) {
        puts("DHT11 initialization failed");
        return 1;
    }

    while (1) {
        if (dht_read(&dev, &temp, &hum) != DHT_OK) {
            puts("Failed to read data from DHT11");
        } else {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
                   temp / 10, temp % 10, hum / 10, hum % 10);
        }

        xtimer_usleep(SLEEP_INTERVAL);
    }

    return 0;
}
