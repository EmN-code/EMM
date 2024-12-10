
#include "xtimer.h"
#include "periph/gpio.h"
#include "dht.h"

#define DHT_PIN             GPIO_PIN(0, 2)
#define DHT_TYPE            DHT11
#define SLEEP_INTERVAL      (5 * 1000000U)  /* 5 seconds */

static dht_params_t dht_params = {
    .pin = DHT_PIN,
    .type = DHT_TYPE,
    .in_mode = DHT_PARAM_PULL,  /* Ensure this is defined */
};

int main(void)
{
    dht_t dev;
    int16_t temp, hum;

    /* Initialize the DHT11 sensor */
    if (dht_init(&dev, &dht_params) == DHT_OK) {
        while (1) {
            /* Read temperature and humidity */
            if (dht_read(&dev, &temp, &hum) == DHT_OK) {
                printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
                       (int)temp / 10, (int)temp % 10,
                       (int)hum / 10, (int)hum % 10);
            } else {
                puts("Failed to read from DHT11 sensor");
            }

            /* Sleep for 5 seconds */
            xtimer_usleep(SLEEP_INTERVAL);
        }
    } else {
        puts("Failed to initialize DHT11 sensor");
    }

    return 0;
}
