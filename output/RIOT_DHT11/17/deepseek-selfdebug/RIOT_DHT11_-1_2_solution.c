
#include "xtimer.h"
#include "periph/gpio.h"
#include "dht.h"

#define DHT_PIN             GPIO_PIN(0, 2)
#define DHT_TYPE            DHT11
#define SAMPLE_INTERVAL     (5 * 1000 * 1000U) /* 5 seconds in microseconds */

static dht_params_t dht_params = {
    .pin = DHT_PIN,
    .type = DHT_TYPE,
    .in_mode = GPIO_IN_PU, /* Use GPIO_IN_PU instead of DHT_PARAM_PULL */
};

int main(void)
{
    dht_t dev;
    int16_t temp, hum;

    /* Initialize the DHT device */
    if (dht_init(&dev, &dht_params) != DHT_OK) {
        puts("DHT initialization failed");
        return 1;
    }

    while (1) {
        /* Read temperature and humidity */
        if (dht_read(&dev, &temp, &hum) == DHT_OK) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
                   temp / 10, temp % 10, hum / 10, hum % 10);
        } else {
            puts("Failed to read data from DHT");
        }

        /* Sleep for 5 seconds */
        xtimer_usleep(SAMPLE_INTERVAL);
    }

    return 0;
}
