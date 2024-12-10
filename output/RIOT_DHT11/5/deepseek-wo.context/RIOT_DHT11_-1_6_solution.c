
#include "xtimer.h"
#include "periph/gpio.h"
#include "dht.h"
#include <stdio.h>

#define DHT_PIN GPIO_PIN(0, 2)
#define INTERVAL (5 * 1000 * 1000U) /* 5 seconds in microseconds */

static dht_t dht_dev;

int main(void)
{
    int16_t temp, hum;

    /* Initialize the DHT11 sensor */
    if (dht_init(&dht_dev, DHT_TYPE_DHT11, DHT_PIN) != DHT_OK) {
        puts("DHT11 initialization failed");
        return 1;
    }

    while (1) {
        /* Read temperature and humidity */
        if (dht_read(&dht_dev, &temp, &hum) != DHT_OK) {
            puts("DHT11 read failed");
        } else {
            printf("Temperature: %d.%d C, Humidity: %d.%d %%\n",
                   temp / 10, temp % 10, hum / 10, hum % 10);
        }

        /* Wait for 5 seconds */
        xtimer_usleep(INTERVAL);
    }

    return 0;
}
