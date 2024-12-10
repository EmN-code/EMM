
#include "xtimer.h"
#include "periph/gpio.h"
#include "dht.h"

#define DHT_PIN             GPIO_PIN(0, 2)
#define DHT_TYPE            DHT11
#define SLEEP_INTERVAL      (5 * 1000000U)  /* 5 seconds */

static dht_t dht_dev;

int main(void)
{
    int16_t temp, hum;

    /* Initialize the DHT11 sensor */
    dht_params_t dht_params = {
        .pin = DHT_PIN,
        .type = DHT_TYPE,
        .in_mode = GPIO_IN_PU  /* Use GPIO_IN_PU instead of DHT_PARAM_PULL */
    };

    if (dht_init(&dht_dev, &dht_params) == DHT_OK) {
        while (1) {
            if (dht_read(&dht_dev, &temp, &hum) == DHT_OK) {
                printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
                       (int)temp / 10, (int)temp % 10,
                       (int)hum / 10, (int)hum % 10);
            } else {
                puts("Failed to read from DHT sensor");
            }

            xtimer_usleep(SLEEP_INTERVAL);
        }
    } else {
        puts("Failed to initialize DHT sensor");
    }

    return 0;
}
