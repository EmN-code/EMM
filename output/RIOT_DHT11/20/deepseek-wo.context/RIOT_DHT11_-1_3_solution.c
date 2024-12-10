
#include <stdio.h>
#include "xtimer.h"
#include "periph/gpio.h"
#include "dht.h"

#define DHT_PIN             GPIO_PIN(0, 2)
#define DHT_TYPE            DHT11
#define SLEEP_INTERVAL      (5 * 1000000U)  /* 5 seconds */

static dht_t dht_dev;

void read_dht(void)
{
    int16_t temp, hum;
    if (dht_read(&dht_dev, &temp, &hum) == DHT_OK) {
        printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
               temp / 10, temp % 10, hum / 10, hum % 10);
    } else {
        puts("Error reading DHT sensor");
    }
}

int main(void)
{
    /* Initialize the DHT11 sensor */
    dht_params_t dht_params = {
        .pin = DHT_PIN,
        .type = DHT_TYPE,
        .in_mode = DHT_PARAM_PULL,  /* Ensure this is defined */
    };

    if (dht_init(&dht_dev, &dht_params) != DHT_OK) {
        puts("DHT11 initialization failed");
        return 1;
    }

    while (1) {
        read_dht();
        xtimer_usleep(SLEEP_INTERVAL);
    }

    return 0;
}
